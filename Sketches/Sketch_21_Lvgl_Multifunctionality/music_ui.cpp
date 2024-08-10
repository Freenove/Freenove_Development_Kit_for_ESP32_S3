#include "main_ui.h"
#include "music_ui.h"
#include "sd_card.h"
#include "stdlib.h"
#include "string.h"
#include "Audio.h"
#include "FS.h"
#include "SD_MMC.h"
#include "lv_img.h"

lvgl_music_ui guider_music_ui;//music ui structure
int music_button_state = 0;   //UI Button status
int music_index_num = 1;      //index number of the music

Audio audio;
int music_task_flag = 0;       //music thread running flag
TaskHandle_t musicTaskHandle;  //music thread task handle

//Click the logo icon, callback function: goes to the main ui interface
static void music_imgbtn_home_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the logo button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        stop_music_task();//Close the thread when exiting the musci ui interface
        if (!lv_obj_is_valid(guider_main_ui.main))
          setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_music_ui.music);
      }
      break;
    default:
      break;
  }
}

//Click the left icon, callback function: play the last song
static void music_imgbtn_left_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the left button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        Serial.println("Play the last song.");
        music_index_num--;
        if (music_index_num < 1)
          music_index_num = list_count_number(list_music);

        stop_music_task();
        lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_pause);
        music_button_state = 0;
        //lv_label_set_text(guider_music_ui.music_label, list_find_node(list_music, music_index_num));
        char *buf_music_name = list_find_node(list_music, music_index_num);
        music_set_label_text(buf_music_name);
        if (buf_music_name != NULL) {
          char buf_music_path[255] = {MUSIC_FOLDER};
          strcat(buf_music_path, "/");
          strcat(buf_music_path, buf_music_name);
          Serial.println(buf_music_path);
          music_load_mp3(buf_music_path);
          start_music_task();
          lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_playing);
          music_button_state = 1;
        }
      }
      break;
    default:
      break;
  }
}

//Click the right icon, callback function: play the next song
static void music_imgbtn_right_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the right button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        Serial.println("Play the next song.");
        music_index_num++;
        if (music_index_num > list_count_number(list_music))
          music_index_num = 1;

        stop_music_task();
        lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_pause);
        music_button_state = 0;
        char *buf_music_name = list_find_node(list_music, music_index_num);
        music_set_label_text(buf_music_name);
        if (buf_music_name != NULL)
        {
          char buf_music_path[255] = {MUSIC_FOLDER};
          strcat(buf_music_path, "/");
          strcat(buf_music_path, buf_music_name);
          Serial.println(buf_music_path);
          music_load_mp3(buf_music_path);
          start_music_task();
          lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_playing);
          music_button_state = 1;
        }
      }
      break;
    default:
      break;
  }
}

//Click the play icon, callback function: play or pause the music
static void music_imgbtn_play_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the play button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        music_button_state = !music_button_state;
        if (music_button_state == 1)
          lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_playing);
        else
          lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_pause);

        //Gets whether a music task currently exists
        int is_task_running = music_task_is_running();
        if (is_task_running == 1) {  //If so, pause or play it
          music_pause_resume();
        } else {
          /*If there is no music thread currently, load the music name first, and then create the audio thread*/
          char *buf_music_name = list_find_node(list_music, music_index_num);
          if (buf_music_name != NULL)
          {
            char buf_music_path[255] = {MUSIC_FOLDER};
            strcat(buf_music_path, "/");
            strcat(buf_music_path, buf_music_name);
            Serial.println(buf_music_path);
            music_load_mp3(buf_music_path);
            start_music_task();
          }
        }
      }
      break;
    default:
      break;
  }
}

//Click the stop icon, callback function: stop the music
static void music_imgbtn_stop_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the stop button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_pause);
        stop_music_task();
        lv_bar_set_value(guider_music_ui.music_bar_time, 0, LV_ANIM_OFF);
        music_button_state = 0;
        Serial.println("The music has stop.");
      }
      break;
    default:
      break;
  }
}

static void music_slider_change_event_handler(lv_event_t * e) {
  lv_obj_t *slider = lv_event_get_target(e);
  char buf[16];
  int volume = (int)lv_slider_get_value(slider);
  lv_snprintf(buf, sizeof(buf), "Volume:%d", volume);
  lv_label_set_text(guider_music_ui.music_slider_label, buf);
  int last_volume = music_read_volume();
  if (volume != last_volume)
    music_set_volume(volume);
}

//Parameter configuration function on the music screen
void setup_scr_music(lvgl_music_ui *ui) {
  ui->music = lv_obj_create(NULL);

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->music, &bg_style, LV_PART_MAIN);

  lv_img_home_init();
  lv_img_left_init();
  lv_img_right_init();
  lv_img_pause_init();
  lv_img_playing_init();
  lv_img_stop_init();
  setup_list_head_music();

  /*Init the pressed style*/
  static lv_style_t style_pr;//Apply for a style
  lv_style_init(&style_pr);  //Initialize it
  lv_style_set_translate_y(&style_pr, 5);//Style: Every time you trigger, move down 5 pixels

  ui->music_imgbtn_home = lv_imgbtn_create(ui->music);
  lv_obj_set_pos(ui->music_imgbtn_home, 80, 20);
  lv_obj_set_size(ui->music_imgbtn_home, 80, 80);
  lv_img_set_src(ui->music_imgbtn_home, &img_home);
  lv_obj_add_style(ui->music_imgbtn_home, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  ui->music_slider_label = lv_label_create(ui->music);
  lv_obj_set_size(ui->music_slider_label, 160, 20);
  lv_obj_align(ui->music_slider_label, LV_ALIGN_CENTER, 40, -30);

  ui->music_slider_valume = lv_slider_create(ui->music);
  lv_obj_set_size(ui->music_slider_valume, 180, 10);
  lv_obj_set_pos(ui->music_slider_valume, 30, 150);
  lv_slider_set_mode(ui->music_slider_valume, LV_SLIDER_MODE_NORMAL);
  lv_slider_set_range(ui->music_slider_valume, 0, 21);
  lv_slider_set_value(ui->music_slider_valume, 10, LV_ANIM_OFF);

  ui->music_label = lv_label_create(ui->music);
  lv_obj_set_pos(ui->music_label, 40, 180);
  lv_obj_set_size(ui->music_label, 160, 20);
  music_set_label_text(list_find_node(list_music, music_index_num));
  lv_label_set_long_mode(ui->music_label, LV_LABEL_LONG_SCROLL_CIRCULAR );
  lv_obj_set_style_text_align(ui->music_label, LV_TEXT_ALIGN_CENTER, 0);

  ui->music_imgbtn_left = lv_imgbtn_create(ui->music);
  lv_obj_set_pos(ui->music_imgbtn_left, 0, 220);
  lv_obj_set_size(ui->music_imgbtn_left, 60, 60);
  lv_img_set_src(ui->music_imgbtn_left, &img_left);
  lv_obj_add_style(ui->music_imgbtn_left, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  ui->music_imgbtn_play = lv_imgbtn_create(ui->music);
  lv_obj_set_pos(ui->music_imgbtn_play, 60, 220);
  lv_obj_set_size(ui->music_imgbtn_play, 60, 60);
  lv_img_set_src(ui->music_imgbtn_play, &img_pause);
  lv_obj_add_style(ui->music_imgbtn_play, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  ui->music_imgbtn_stop = lv_imgbtn_create(ui->music);
  lv_obj_set_pos(ui->music_imgbtn_stop, 120, 220);
  lv_obj_set_size(ui->music_imgbtn_stop, 60, 60);
  lv_img_set_src(ui->music_imgbtn_stop, &img_stop);
  lv_obj_add_style(ui->music_imgbtn_stop, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  ui->music_imgbtn_right = lv_imgbtn_create(ui->music);
  lv_obj_set_pos(ui->music_imgbtn_right, 180, 220);
  lv_obj_set_size(ui->music_imgbtn_right, 60, 60);
  lv_img_set_src(ui->music_imgbtn_right, &img_right);
  lv_obj_add_style(ui->music_imgbtn_right, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  ui->music_bar_time = lv_bar_create(ui->music);
  lv_obj_set_size(ui->music_bar_time, 240, 5);
  lv_obj_set_pos(ui->music_bar_time, 0, 290);
  lv_slider_set_mode(ui->music_bar_time, LV_SLIDER_MODE_NORMAL);
  lv_bar_set_range(ui->music_bar_time, 0, 100);
  lv_bar_set_value(ui->music_bar_time, 0, LV_ANIM_OFF);

  lv_obj_add_event_cb(ui->music_imgbtn_home, music_imgbtn_home_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->music_imgbtn_left, music_imgbtn_left_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->music_imgbtn_right, music_imgbtn_right_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->music_imgbtn_play, music_imgbtn_play_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->music_imgbtn_stop, music_imgbtn_stop_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->music_slider_valume, music_slider_change_event_handler, LV_EVENT_ALL, NULL);
}

//Set the label display content
void music_set_label_text(char *text) {
  if (text != NULL) {
    lv_label_set_text(guider_music_ui.music_label, text);
  }
  else {
    lv_label_set_text(guider_music_ui.music_label, "The music folder has no files.");
  }
}

//Initialize the audio interface
int music_iis_init(void) {
  return audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
}
//Set the volume: 0-21
void music_set_volume(int volume) {
  audio.setVolume(volume);
}
//Query volume
int music_read_volume(void) {
  return audio.getVolume();
}
//load the mp3
void music_load_mp3(char *name) {
  audio.connecttoFS(SD_MMC, name);
}
//Pause/play the music
void music_pause_resume(void) {
  audio.pauseResume();
}
//Stop the music
void music_stop(void) {
  audio.stopSong();
}
//Whether the music is running
bool music_is_running(void) {
  return audio.isRunning();
}
//Gets how long the music player has been playing
long music_get_total_playing_time(void) {
  return (long)audio.getTotalPlayingTime() / 1000;
}
//Obtain the playing time of the music file
long music_get_file_duration(void) {
  return (long)audio.getAudioFileDuration();
}
//Set play position
bool music_set_play_position(int second) {
  return audio.setAudioPlayPosition((uint16_t)second);
}
//Gets the current playing time of the music
long music_read_play_position(void) {
  return audio.getAudioCurrentTime();
}
//Non-blocking music execution function
void music_loop(void) {
  audio.loop();
}

//Init SPIFS
void music_fs_init(void){
  SPIFFS.begin();
}

//Close SPIFS
void music_fs_end(void){
  SPIFFS.end();
}

//Load FS mp3
void music_load_fs_mp3(char *name){
  audio.connecttoFS(SPIFFS, name);
}

//music player thread
void loopTask_music(void *pvParameters) {
  Serial.println("loopTask_music start...");
  int temp = 0;
  while (music_task_flag == 1) {
    music_loop();
    int t1 = music_get_total_playing_time();//Gets how long the music player has been playing
    int t2 = music_get_file_duration();     //Gets the playing time of the music file
    int t3 = music_read_play_position();    //Gets the current playing time of the music
    if (temp == 1) {
      int t4 = map(t3, 0, t2, 0, 100);
      if (t4 <= 100)
        lv_bar_set_value(guider_music_ui.music_bar_time, t4, LV_ANIM_OFF);
      //Serial.printf("t1: %d\t t2: %d\t t3: %d\t t4: %d\r\n", t1, t2, t3, t4);
    }
    if ((t1 < t2) && (t2 > 0) && (temp == 0)) { //The music starts to play
      lv_bar_set_value(guider_music_ui.music_bar_time, 0, LV_ANIM_OFF);
      temp = 1;
    } else if ((t2 == 0) && (temp == 1)) {      //The music stop to play
      temp = 0;
      music_task_flag = 0;
      lv_img_set_src(guider_music_ui.music_imgbtn_play, &img_pause);
      music_button_state = 0;
      break;
    }
  }
  music_stop();
  Serial.println("loopTask_music stop...");
  vTaskDelete(musicTaskHandle);
}

//Create music task thread
void start_music_task(void) {
  if (music_task_flag == 0) {
    music_task_flag = 1;
    xTaskCreate(loopTask_music, "loopTask_music", 8192, NULL, 1, &musicTaskHandle);
  } else {
    Serial.println("loopTask_music is running...");
  }
}

//Close the music thread
void stop_music_task(void) {
  if (music_task_flag == 1) {
    music_task_flag = 0;
    while (1) {
      if (eTaskGetState(musicTaskHandle) == eDeleted) {
        break;
      }
      vTaskDelay(10);
    }
    Serial.println("loopTask_music deleted!");
  }
}

//Check whether the thread is running
int music_task_is_running(void) {
  return music_task_flag;
}

// optional
void audio_info(const char *info) {
  Serial.print("info        ");
  Serial.println(info);
}
void audio_eof_mp3(const char *info) {
  Serial.print("eof_mp3     ");
  Serial.println(info);
}
