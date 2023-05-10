#include "main_ui.h"
#include "second_main_ui.h"
#include "lv_img.h"

#include "ws2812_ui.h"
#include "sound_ui.h"
#include "wifi_ui.h"

lvgl_second_main_ui guider_second_main_ui;//second_main ui structure

//Click the second_main interface logo, callback function: do nothing
static void second_main_imgbtn_logo_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the logo button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_main_ui.main))
          setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_second_main_ui.second_main);
      }
      break;
    default:
      break;
  }
}

//Click the ws2812 icon, callback function: goes to the ws2812 ui interface
static void second_main_imgbtn_ws2812_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the ws2812 button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_ws2812_ui.ws2812))
          setup_scr_ws2812(&guider_ws2812_ui);
        lv_scr_load(guider_ws2812_ui.ws2812);
        lv_obj_del(guider_second_main_ui.second_main);
      }
      break;
    default:
      break;
  }
}
//Click the buzzer icon, callback function: goes to the buzzer ui interface
static void second_main_imgbtn_buzzer_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the buzzer button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_sound_ui.sound))
          setup_scr_sound(&guider_sound_ui);
        lv_scr_load(guider_sound_ui.sound);
        lv_obj_del(guider_second_main_ui.second_main);
      }
      break;
    default:
      break;
  }
}

//Click the folder icon, callback function: goes to the folder ui interface
static void second_main_imgbtn_folder_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the folder button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_wifi_ui.wifi))
          setup_scr_wifi(&guider_wifi_ui);
        lv_scr_load(guider_wifi_ui.wifi);
        lv_obj_del(guider_second_main_ui.second_main);
      }
      break;
    default:
      break;
  }
}

//Parameter configuration function on the second_main screen
void setup_scr_second_main(lvgl_second_main_ui *ui) {
  //Write codes second_main
  ui->second_main = lv_obj_create(NULL);

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->second_main, &bg_style, LV_PART_MAIN);

  lv_img_freenove_init();
  lv_img_buzzer_init();
  lv_img_ws2812_init();
  lv_img_folder_init();

  /*Init the pressed style*/
  static lv_style_t style_pr;//Apply for a style
  lv_style_init(&style_pr);  //Initialize it
  lv_style_set_translate_y(&style_pr, 5);//Style: Every time you trigger, move down 5 pixels

  //Write codes second_main_imgbtn_logo
  ui->second_main_imgbtn_logo = lv_imgbtn_create(ui->second_main);
  lv_obj_set_pos(ui->second_main_imgbtn_logo, 40, 20);
  lv_obj_set_size(ui->second_main_imgbtn_logo, 160, 90);
  lv_img_set_src(ui->second_main_imgbtn_logo, &img_freenove);
  lv_obj_add_style(ui->second_main_imgbtn_logo, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  //Write codes second_main_imgbtn_ws2812
  ui->second_main_imgbtn_ws2812 = lv_imgbtn_create(ui->second_main);
  lv_obj_set_pos(ui->second_main_imgbtn_ws2812, 20, 135);
  lv_obj_set_size(ui->second_main_imgbtn_ws2812, 80, 80);
  lv_img_set_src(ui->second_main_imgbtn_ws2812, &img_ws2812);
  lv_obj_add_style(ui->second_main_imgbtn_ws2812, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  //Write codes second_main_imgbtn_buzzer
  ui->second_main_imgbtn_buzzer = lv_imgbtn_create(ui->second_main);
  lv_obj_set_pos(ui->second_main_imgbtn_buzzer, 140, 135);
  lv_obj_set_size(ui->second_main_imgbtn_buzzer, 80, 80);
  lv_img_set_src(ui->second_main_imgbtn_buzzer, &img_buzzer);
  lv_obj_add_style(ui->second_main_imgbtn_buzzer, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  //Write codes second_main_imgbtn_folder
  ui->second_main_imgbtn_folder = lv_imgbtn_create(ui->second_main);
  lv_obj_set_pos(ui->second_main_imgbtn_folder, 20, 225);
  lv_obj_set_size(ui->second_main_imgbtn_folder, 80, 80);
  lv_img_set_src(ui->second_main_imgbtn_folder, &img_folder);
  lv_obj_add_style(ui->second_main_imgbtn_folder, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  lv_obj_add_event_cb(ui->second_main_imgbtn_logo, second_main_imgbtn_logo_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->second_main_imgbtn_ws2812, second_main_imgbtn_ws2812_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->second_main_imgbtn_buzzer, second_main_imgbtn_buzzer_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->second_main_imgbtn_folder, second_main_imgbtn_folder_event_handler, LV_EVENT_ALL, NULL);
}

//Load Ui Screen function
void setup_ui(lvgl_second_main_ui *ui) {
  setup_scr_second_main(ui);
  lv_scr_load(ui->second_main);
}
