#include "second_main_ui.h"
#include "wifi_ui.h"
#include "sd_card.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "command.h"
#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"
#include "esp_camera.h"
#include "camera.h"

wifi_parameter wifi_info;
wifi_parameter send_info;
lvgl_wifi_ui guider_wifi_ui;  //wifi ui structure
char wifi_name[FILE_NAME_LENGTH] = { "WiFi name" };
char wifi_password[FILE_NAME_LENGTH] = { "WiFi password" };

TaskHandle_t wifiTaskHandle;     //wifi thread task handle
static int wifi_task_flag = 0;   //WiFi thread running flag
TaskHandle_t videoTaskHandle;    //video thread task handle
static int wifi_video_flag = 0;  //video thread running flag
camera_fb_t *fb_video = NULL;          //data structure of camera frame buffer

WiFiServer server_cmd(WIFI_CMD_PORT);
WiFiServer server_video(WIFI_VIDEO_PORT);
WiFiClient client_cmd;
WiFiClient client_video;

static void wifi_keyboard_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *kb = lv_event_get_target(e);
  if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
}

static void wifi_textarea_name_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  if (code == LV_EVENT_FOCUSED)  //When focusing the textarea
  {
    lv_keyboard_set_textarea(guider_wifi_ui.wifi_keyboard, ta);           //Associate keyboard input to a textarea
    lv_obj_clear_flag(guider_wifi_ui.wifi_keyboard, LV_OBJ_FLAG_HIDDEN);  //show the keyboard
  }
  if (code == LV_EVENT_DEFOCUSED)  //When defocusing the textarea
  {
    lv_keyboard_set_textarea(guider_wifi_ui.wifi_keyboard, ta);         //Associate keyboard input to a textarea
    lv_obj_add_flag(guider_wifi_ui.wifi_keyboard, LV_OBJ_FLAG_HIDDEN);  //show the keyboard
    Serial.printf("textarea content are %s\r\n", lv_textarea_get_text(ta));
  }
}

static void wifi_textarea_password_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  if (code == LV_EVENT_FOCUSED)  //When focusing the textarea
  {
    lv_keyboard_set_textarea(guider_wifi_ui.wifi_keyboard, ta);           //Associate keyboard input to a textarea
    lv_obj_clear_flag(guider_wifi_ui.wifi_keyboard, LV_OBJ_FLAG_HIDDEN);  //show the keyboard
  }
  if (code == LV_EVENT_DEFOCUSED)  //When defocusing the textarea
  {
    lv_keyboard_set_textarea(guider_wifi_ui.wifi_keyboard, ta);         //Associate keyboard input to a textarea
    lv_obj_add_flag(guider_wifi_ui.wifi_keyboard, LV_OBJ_FLAG_HIDDEN);  //show the keyboard
    Serial.printf("textarea content are %s\r\n", lv_textarea_get_text(ta));
  }
}

static void wifi_btn_save_event_hander(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.printf("Clicked the save button.\r\n");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        wifi_config_file_delete();
        wifi_config_save();
      }
      break;
    default:
      break;
  }
}

static void wifi_btn_connect_event_hander(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.printf("Clicked the connect button.\r\n");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!wifi_is_connect()) {
          memset(wifi_name, 0, FILE_NAME_LENGTH);
          memset(wifi_password, 0, FILE_NAME_LENGTH);
          strcat(wifi_name, lv_textarea_get_text(guider_wifi_ui.wifi_textarea_name));
          strcat(wifi_password, lv_textarea_get_text(guider_wifi_ui.wifi_textarea_password));
          lv_label_set_text(guider_wifi_ui.wifi_lable_connect, "Config...");

          int wifi_state = wifi_connect(wifi_name, wifi_password);
          if (!wifi_state) {
            Serial.printf("%s connect failed.\r\n", wifi_name);
            lv_textarea_set_text(guider_wifi_ui.wifi_textarea_local_ip, "NULL");
          } else {
            String wifi_ip = WiFi.localIP().toString();
            lv_textarea_set_text(guider_wifi_ui.wifi_textarea_local_ip, wifi_ip.c_str());
            lv_label_set_text(guider_wifi_ui.wifi_lable_connect, "Disconnect");
            create_wifi_task();
          }
        } else {
          lv_label_set_text(guider_wifi_ui.wifi_lable_connect, "Connect");
          lv_textarea_set_text(guider_wifi_ui.wifi_textarea_local_ip, "NULL");
          lv_textarea_set_text(guider_wifi_ui.wifi_textarea_remote_ip, "NULL");
          wifi_disconect();
          stop_wifi_task();
        }
      }
      break;
    default:
      break;
  }
}

static void wifi_screen_gesture_event_hander(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_GESTURE) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    switch (dir) {
      case LV_DIR_LEFT:
      case LV_DIR_RIGHT:
        stop_video_task();
        stop_wifi_task();
        wifi_disconect();
        if (!lv_obj_is_valid(guider_second_main_ui.second_main))
          setup_scr_second_main(&guider_second_main_ui);
        lv_scr_load(guider_second_main_ui.second_main);
        lv_obj_del(guider_wifi_ui.wifi);
        break;

      case LV_DIR_TOP:
      case LV_DIR_BOTTOM:
        break;
    }
  }
}

void setup_scr_wifi(lvgl_wifi_ui *ui) {
  ui->wifi = lv_obj_create(NULL);
  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->wifi, &bg_style, LV_PART_MAIN);

  ui->wifi_lable_name = lv_label_create(ui->wifi);
  lv_obj_set_pos(ui->wifi_lable_name, 15, 20);
  lv_obj_set_size(ui->wifi_lable_name, 40, 40);
  lv_label_set_text(ui->wifi_lable_name, "WiFi:");

  ui->wifi_textarea_name = lv_textarea_create(ui->wifi);
  lv_textarea_set_one_line(ui->wifi_textarea_name, true);
  lv_obj_add_state(ui->wifi_textarea_name, LV_STATE_DEFAULT);  //The cursor is displayed when focusing, but not when defocusing
  lv_obj_set_pos(ui->wifi_textarea_name, 70, 10);
  lv_obj_set_size(ui->wifi_textarea_name, 155, 40);
  lv_textarea_set_text(ui->wifi_textarea_name, wifi_name);

  ui->wifi_lable_password = lv_label_create(ui->wifi);
  lv_obj_set_pos(ui->wifi_lable_password, 15, 70);
  lv_obj_set_size(ui->wifi_lable_password, 40, 40);
  lv_label_set_text(ui->wifi_lable_password, "Psw:");

  ui->wifi_textarea_password = lv_textarea_create(ui->wifi);
  lv_textarea_set_one_line(ui->wifi_textarea_password, true);
  lv_obj_add_state(ui->wifi_textarea_password, LV_STATE_DEFAULT);  //The cursor is displayed when focusing, but not when defocusing
  lv_obj_set_pos(ui->wifi_textarea_password, 70, 60);
  lv_obj_set_size(ui->wifi_textarea_password, 155, 40);
  lv_textarea_set_text(ui->wifi_textarea_password, wifi_password);

  /*Init the pressed style*/
  static lv_style_t style_pr;              //Apply for a style
  lv_style_init(&style_pr);                //Initialize it
  lv_style_set_translate_y(&style_pr, 5);  //Style: Every time you trigger, move down 5 pixels

  ui->wifi_btn_connect = lv_btn_create(ui->wifi);
  lv_obj_set_pos(ui->wifi_btn_connect, 15, 125);
  lv_obj_set_size(ui->wifi_btn_connect, 95, 40);
  lv_obj_add_style(ui->wifi_btn_connect, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed
  ui->wifi_lable_connect = lv_label_create(ui->wifi_btn_connect);
  lv_label_set_text(ui->wifi_lable_connect, "Connect");
  lv_obj_center(ui->wifi_lable_connect);

  ui->wifi_btn_save = lv_btn_create(ui->wifi);
  lv_obj_set_pos(ui->wifi_btn_save, 130, 125);
  lv_obj_set_size(ui->wifi_btn_save, 95, 40);
  lv_obj_add_style(ui->wifi_btn_save, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed
  ui->wifi_lable_home = lv_label_create(ui->wifi_btn_save);
  lv_label_set_text(ui->wifi_lable_home, "Save");
  lv_obj_center(ui->wifi_lable_home);

  ui->wifi_lable_local_ip = lv_label_create(ui->wifi);
  lv_obj_set_pos(ui->wifi_lable_local_ip, 15, 210);
  lv_obj_set_size(ui->wifi_lable_local_ip, 80, 40);
  lv_label_set_text(ui->wifi_lable_local_ip, "Local IP:");

  ui->wifi_textarea_local_ip = lv_textarea_create(ui->wifi);
  lv_textarea_set_one_line(ui->wifi_textarea_local_ip, true);
  lv_obj_add_state(ui->wifi_textarea_local_ip, LV_STATE_DISABLED);
  lv_obj_set_pos(ui->wifi_textarea_local_ip, 105, 200);
  lv_obj_set_size(ui->wifi_textarea_local_ip, 120, 40);
  lv_textarea_set_text(ui->wifi_textarea_local_ip, "NULL");

  ui->wifi_lable_remote_ip = lv_label_create(ui->wifi);
  lv_obj_set_pos(ui->wifi_lable_remote_ip, 15, 270);
  lv_obj_set_size(ui->wifi_lable_remote_ip, 80, 40);
  lv_label_set_text(ui->wifi_lable_remote_ip, "Remote IP:");

  ui->wifi_textarea_remote_ip = lv_textarea_create(ui->wifi);
  lv_textarea_set_one_line(ui->wifi_textarea_remote_ip, true);
  lv_obj_add_state(ui->wifi_textarea_remote_ip, LV_STATE_DISABLED);
  lv_obj_set_pos(ui->wifi_textarea_remote_ip, 105, 260);
  lv_obj_set_size(ui->wifi_textarea_remote_ip, 120, 40);
  lv_textarea_set_text(ui->wifi_textarea_remote_ip, "NULL");

  ui->wifi_keyboard = lv_keyboard_create(ui->wifi);
  lv_obj_set_size(ui->wifi_keyboard, LV_HOR_RES, LV_VER_RES / 2);
  lv_obj_add_flag(ui->wifi_keyboard, LV_OBJ_FLAG_HIDDEN);

  lv_obj_add_event_cb(ui->wifi_textarea_name, wifi_textarea_name_event_handler, LV_EVENT_ALL, ui->wifi_keyboard);
  lv_obj_add_event_cb(ui->wifi_textarea_password, wifi_textarea_password_event_handler, LV_EVENT_ALL, ui->wifi_keyboard);
  lv_obj_add_event_cb(ui->wifi_keyboard, wifi_keyboard_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->wifi_btn_save, wifi_btn_save_event_hander, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->wifi_btn_connect, wifi_btn_connect_event_hander, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->wifi, wifi_screen_gesture_event_hander, LV_EVENT_ALL, NULL);
  
  camera_init_jpg();
  wifi_config_save();
}

int wifi_config_file_is_exists(char *path) {
  return sdcard_is_exists(path);
}

int wifi_config_file_delete(char *path) {
  return sdcard_remeve_file(path);
}

void wifi_config_save(void) {
  if (!wifi_config_file_is_exists()) {
    Serial.printf("wifi config file is not exists.\r\n");
    memset(wifi_name, 0, FILE_NAME_LENGTH);
    memset(wifi_password, 0, FILE_NAME_LENGTH);
    strcat(wifi_name, lv_textarea_get_text(guider_wifi_ui.wifi_textarea_name));
    strcat(wifi_password, lv_textarea_get_text(guider_wifi_ui.wifi_textarea_password));
    sdcard_wifi_file_write((uint8_t *)wifi_name, (uint8_t *)wifi_password);
  } else {
    memset(wifi_name, 0, FILE_NAME_LENGTH);
    memset(wifi_password, 0, FILE_NAME_LENGTH);
    sdcard_wifi_file_read((uint8_t *)wifi_name, (uint8_t *)wifi_password);
    lv_textarea_set_text(guider_wifi_ui.wifi_textarea_name, wifi_name);
    lv_textarea_set_text(guider_wifi_ui.wifi_textarea_password, wifi_password);
  }
}

int wifi_connect(char *wifi_name, char *wifi_passward) {
  WiFi.begin(wifi_name, wifi_passward);
  int time = 0;
  while (!wifi_is_connect()) {
    delay(500);
    Serial.print(".");
    time++;
    if (time > 10)
      return 0;
  }
  time = 0;
  while (WiFi.STA.hasIP() != true) {
    delay(500);
    Serial.print(".");
    time++;
    if (time > 10)
      return 0;
  }
  Serial.println();
  return 1;
}

int wifi_is_connect(void) {
  if (WiFi.isConnected() == true) {
    return 1;
  } else {
    return 0;
  }
}

void wifi_disconect(void) {
  WiFi.disconnect(true, true);
}

//Create wifi task thread
void create_wifi_task(void) {
  if (wifi_task_flag == 0) {
    wifi_task_flag = 1;
    xTaskCreate(loopTask_wifi, "loopTask_wifi", 10240, NULL, 1, &wifiTaskHandle);
  } else {
    Serial.println("loopTask_wifi is running...");
  }
}

//Close the wifi thread
void stop_wifi_task(void) {
  if (wifi_task_flag == 1) {
    wifi_task_flag = 0;
    while (1) {
      if (eTaskGetState(wifiTaskHandle) == eDeleted) {
        break;
      }
      vTaskDelay(10);
    }
    Serial.println("loopTask_wifi deleted!");
  }
}

//wifi thread
void loopTask_wifi(void *pvParameters) {
  Serial.printf("loopTask_wifi start...\r\n");
  server_cmd.begin(WIFI_CMD_PORT);
  while (wifi_task_flag) {
    client_cmd = server_cmd.available();
    if (client_cmd) {
      String remote_ip = client_cmd.remoteIP().toString();
      lv_textarea_set_text(guider_wifi_ui.wifi_textarea_remote_ip, remote_ip.c_str());
      int wifi_write_state = 0;
      while (client_cmd.connected()&&wifi_task_flag==1) {  // loop while the client_cmd's connected
        if (client_cmd.available()) {   // if there's bytes to read from the client_cmd,
          if (wifi_write_state == 1) {
            wifi_write_file_to_sdcard();
            wifi_write_state = 0;
          } else {
            int count = 0;
            String wifiInputString = "";
            while (count < 5) {
              char buf = client_cmd.read();
              wifiInputString += buf;
              if (buf == INTERVAL_CHAR)
                count++;
            }
            wifi_message_parse(wifiInputString);
            wifiInputString = "";
            switch (wifi_info.command) {
              case READ_MODE:
                wifi_read_file_from_sdcard();
                break;
              case WRITE_MODE:
                wifi_write_state = 1;
                break;
              case DELETE_MODE:
                wifi_delete_file_from_sdcard();
                break;
              case LIST_MODE:
                wifi_read_list_from_sdcard();
                break;
              case VIDEO_MODE:
                if (wifi_info.size == 1) {
                  create_video_task();
                } else {
                  stop_video_task();
                }
                break;
              default:
                break;
            }
            if (wifi_write_state != 1) {
              while (client_cmd.available()) {
                client_cmd.read();
              }
            }
          }
        }
      }
      client_cmd.stop();
      lv_textarea_set_text(guider_wifi_ui.wifi_textarea_remote_ip, "NULL");
    }
  }
  vTaskDelete(wifiTaskHandle);
}

//Create video task thread
void create_video_task(void) {
  if (wifi_video_flag == 0) {
    wifi_video_flag = 1;
    xTaskCreate(loopTask_video, "loopTask_video", 8192, NULL, 1, &videoTaskHandle);
  } else {
    Serial.println("loopTask_video is running...");
  }
}

//Close the video thread
void stop_video_task(void) {
  if (wifi_video_flag == 1) {
    wifi_video_flag = 0;
    while (1) {
      if (eTaskGetState(videoTaskHandle) == eDeleted) {
        break;
      }
      vTaskDelay(10);
    }
    Serial.println("loopTask_video deleted!");
  }
}

//video thread
void loopTask_video(void *pvParameters) {
  Serial.printf("loopTask_video start...\r\n");
  server_video.begin(WIFI_VIDEO_PORT);
  while (wifi_video_flag) {
    client_video = server_video.available();
    if (client_video) {
      Serial.println("Video Server connected to a client.");      // print a message out the serial port
      while (client_video.connected() && wifi_video_flag == 1) {  // loop while the client_video's connected
        fb_video = esp_camera_fb_get();
        if (fb_video != NULL) {
          uint8_t slen[4];
          slen[0] = fb_video->len >> 0;
          slen[1] = fb_video->len >> 8;
          slen[2] = fb_video->len >> 16;
          slen[3] = fb_video->len >> 24;
          client_video.write(slen, 4);
          client_video.write(fb_video->buf, fb_video->len);
        } else {
          Serial.println("Camera Error");
        }
        esp_camera_fb_return(fb_video);
      }
      client_video.stop();
    }
  }
  server_video.stop();
  vTaskDelete(videoTaskHandle);
}

void wifi_message_parse(String msg) {
  String wifiInputString = msg;

  int index = wifiInputString.indexOf(INTERVAL_CHAR);
  wifi_info.command = *wifiInputString.substring(0, index).c_str();
  wifiInputString = wifiInputString.substring(index + 1);

  index = wifiInputString.indexOf(INTERVAL_CHAR);
  memset(wifi_info.path, 0, 256);
  strcat(wifi_info.path, (wifiInputString.substring(0, index)).c_str());
  wifiInputString = wifiInputString.substring(index + 1);

  index = wifiInputString.indexOf(INTERVAL_CHAR);
  memset(wifi_info.name, 0, 256);
  strcat(wifi_info.name, (wifiInputString.substring(0, index)).c_str());
  wifiInputString = wifiInputString.substring(index + 1);

  index = wifiInputString.indexOf(INTERVAL_CHAR);
  wifi_info.size = (wifiInputString.substring(0, index)).toInt();
  wifiInputString = wifiInputString.substring(index + 1);

  index = wifiInputString.indexOf(INTERVAL_CHAR);
  wifi_info.position = (wifiInputString.substring(0, index)).toInt();
  Serial.printf("wifi_info:%c %s %s %d %d\r\n", wifi_info.command, wifi_info.path, wifi_info.name, wifi_info.size, wifi_info.position);
}

void wifi_ack(int state) {
  if (state == 1)
    client_cmd.write("OK\r\n", 4);
  else
    client_cmd.write("ERR\r\n", 5);
}

void wifi_send_massage(wifi_parameter info) {
  char buf[1024] = { 0 };
  memset(buf, 0, 1024);
  strncat(buf, &info.command, 1);
  strcat(buf, "#");
  strcat(buf, info.path);
  strcat(buf, "#");
  strcat(buf, info.name);
  strcat(buf, "#");
  char buf_size[10];
  itoa(info.size, buf_size, 10);
  strcat(buf, buf_size);
  strcat(buf, "#");
  char buf_position[10];
  itoa(info.position, buf_position, 10);
  strcat(buf, buf_position);
  strcat(buf, "#\r\n");
  client_cmd.printf("%s", buf);
}

void wifi_read_file_from_sdcard(void) {
  int is_exists = sdcard_is_exists(wifi_info.path);
  if (!is_exists) {
    Serial.printf("Path is not exists.\r\n");
    wifi_ack(0);
    return;
  }
  char path_name_buf[255] = { 0 };
  memset(path_name_buf, 0, 255);
  strcat(path_name_buf, wifi_info.path);
  strcat(path_name_buf, "/");
  strcat(path_name_buf, wifi_info.name);
  is_exists = sdcard_is_exists(path_name_buf);
  if (!is_exists) {
    wifi_ack(0);
    return;
  }
  wifi_send_massage(wifi_info);

  int count = wifi_info.size / AREA_SIZE;
  int rest = wifi_info.size % AREA_SIZE;
  uint8_t *read_file_buf = (uint8_t *)malloc(AREA_SIZE);
  if (count >= 1) {
    for (int i = 0; i < count; i++) {
      sdcard_read_file(path_name_buf, read_file_buf, AREA_SIZE, (SeekMode)(SeekSet), (i * AREA_SIZE));
      client_cmd.write(read_file_buf, AREA_SIZE);
    }
    sdcard_read_file(path_name_buf, read_file_buf, rest, (SeekMode)(SeekSet), (count * AREA_SIZE));
    client_cmd.write(read_file_buf, rest);
  } else {
    sdcard_read_file(path_name_buf, read_file_buf, wifi_info.size, (SeekMode)(SeekSet), 0);
    client_cmd.write(read_file_buf, wifi_info.size);
  }
  delay(300);
  wifi_ack(1);
}

void wifi_write_file_to_sdcard(void) {
  uint8_t *write_file_buf;
  int count;
  int rest;
  if (wifi_info.size > 1024) {
    count = wifi_info.size / wifi_info.position;
    rest = wifi_info.size % wifi_info.position;
    write_file_buf = (uint8_t *)malloc(wifi_info.position);
    for (int i = 0; i < count; i++) {
      client_cmd.read(write_file_buf, wifi_info.position);
      if (sdcard_write(wifi_info.path, wifi_info.name, write_file_buf, wifi_info.position))
        Serial.printf("Wifi write file success:%ld.\r\n", (i + 1) * wifi_info.position);
      else
        Serial.printf("Wifi write file failed:%ld.\r\n", (i + 1) * wifi_info.position);
    }
    client_cmd.read(write_file_buf, rest);
    if (sdcard_write(wifi_info.path, wifi_info.name, write_file_buf, rest))
      Serial.printf("Wifi write file success:%ld.\r\n", count * wifi_info.position + rest);
    else
      Serial.printf("Wifi write file failed:%ld.\r\n", count * wifi_info.position + rest);
  } else {
    write_file_buf = (uint8_t *)malloc(wifi_info.size);
    client_cmd.read(write_file_buf, wifi_info.size);
    if (sdcard_write(wifi_info.path, wifi_info.name, write_file_buf, wifi_info.size))
      Serial.printf("Wifi write file success:%ld.\r\n", wifi_info.size);
    else
      Serial.printf("Wifi write file failed:%ld.\r\n", wifi_info.size);
  }
  free(write_file_buf);
  wifi_ack(1);
}

void wifi_delete_file_from_sdcard(void) {
  int delete_state;
  int failed_count = 0;
  int is_exists = sdcard_is_exists(wifi_info.path);
  if (!is_exists) {
    Serial.printf("Path is not exists.\r\n");
    wifi_ack(1);
    return;
  }
  if (strncmp(wifi_info.name, "all", 3) == 0) {
    do {
      delete_state = sdcard_remove_dir(wifi_info.path);
      failed_count++;
    } while ((!delete_state) && (failed_count < 5));
    if (!delete_state) {
      Serial.printf("delete dir %s failed.", wifi_info.path);
      wifi_ack(0);
      return;
    } else {
      Serial.printf("delete dir %s success.", wifi_info.path);
      wifi_ack(1);
      return;
    }
  } else {
    char path_name_buf[255] = { 0 };
    memset(path_name_buf, 0, 255);
    strcat(path_name_buf, wifi_info.path);
    strcat(path_name_buf, "/");
    strcat(path_name_buf, wifi_info.name);
    is_exists = sdcard_is_exists(path_name_buf);
    if (!is_exists) {
      wifi_ack(1);
      return;
    } else {
      do {
        delete_state = sdcard_remeve_file(path_name_buf);
        failed_count++;
      } while ((!delete_state) && (failed_count < 5));
      if (!delete_state) {
        Serial.printf("delete %s failed.", path_name_buf);
        wifi_ack(0);
        return;
      } else {
        wifi_ack(1);
      }
    }
  }
}

void wifi_read_list_from_sdcard(void) {
  int is_exists = sdcard_is_exists(wifi_info.path);
  if (!is_exists) {
    Serial.printf("Path is not exists.\r\n");
    wifi_ack(0);
    return;
  }
  list_link *phead = sdcard_get_file_parameter(wifi_info.path);
  int phead_count = list_count_number(phead);
  if (phead_count == 0) {
    memset(wifi_info.name, 0, 256);
    strcat(wifi_info.name, "None");
    wifi_send_massage(wifi_info);
  } else {
    list_link *temp = phead->file_next;
    while (temp != NULL) {
      Serial.printf("%s:%d\r\n", temp->file_name, temp->file_size);
      memset(wifi_info.name, 0, 256);
      strcat(wifi_info.name, temp->file_name);
      wifi_info.size = temp->file_size;
      wifi_info.position = temp->file_index;
      wifi_send_massage(wifi_info);
      temp = temp->file_next;
    }
  }
}
