#include "main_ui.h"
#include "second_main_ui.h"
#include "lv_img.h"

lvgl_main_ui guider_main_ui;  //main ui structure

//Click the main interface logo, callback function: do nothing
static void main_imgbtn_logo_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the logo button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_second_main_ui.second_main))
          setup_scr_second_main(&guider_second_main_ui);
        lv_scr_load(guider_second_main_ui.second_main);
        lv_obj_del(guider_main_ui.main);
      }
      break;
    default:
      break;
  }
}

//Click the camera icon, callback function: goes to the camera ui interface
static void main_imgbtn_camera_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the camera button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_camera_ui.camera))
          setup_scr_camera(&guider_camera_ui);
        lv_scr_load(guider_camera_ui.camera);
        lv_obj_del(guider_main_ui.main);
      }
      break;
    default:
      break;
  }
}

//Click the picture icon, callback function: goes to the picture ui interface
static void main_imgbtn_picture_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the picture button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_picture_ui.picture))
          setup_scr_picture(&guider_picture_ui);
        lv_scr_load(guider_picture_ui.picture);
        lv_obj_del(guider_main_ui.main);
      }
      break;
    default:
      break;
  }
}

//Click the music icon, callback function: goes to the music ui interface
static void main_imgbtn_music_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the music button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_music_ui.music))
          setup_scr_music(&guider_music_ui);
        lv_scr_load(guider_music_ui.music);
        lv_obj_del(guider_main_ui.main);
      }
      break;
    default:
      break;
  }
}

//Click the heartrate icon, callback function: goes to the heartrate ui interface
static void main_imgbtn_heartrate_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the heartrate button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_heartrate_ui.heartrate))
          setup_scr_heartrate(&guider_heartrate_ui);
        lv_scr_load(guider_heartrate_ui.heartrate);
        lv_obj_del(guider_main_ui.main);
      }
      break;
    default:
      break;
  }
}

//Parameter configuration function on the main screen
void setup_scr_main(lvgl_main_ui *ui) {
  //Write codes main
  ui->main = lv_obj_create(NULL);

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->main, &bg_style, LV_PART_MAIN);

  lv_img_freenove_init();
  lv_img_camera_init();
  lv_img_music_init();
  lv_img_heartrate_init();
  lv_img_picture_init();

  /*Init the pressed style*/
  static lv_style_t style_pr;              //Apply for a style
  lv_style_init(&style_pr);                //Initialize it
  lv_style_set_translate_y(&style_pr, 5);  //Style: Every time you trigger, move down 5 pixels

  //Write codes main_imgbtn_logo
  ui->main_imgbtn_logo = lv_imgbtn_create(ui->main);
  lv_obj_set_pos(ui->main_imgbtn_logo, 40, 20);
  lv_obj_set_size(ui->main_imgbtn_logo, 160, 90);
  lv_img_set_src(ui->main_imgbtn_logo, &img_freenove);
  lv_obj_add_style(ui->main_imgbtn_logo, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed

  //Write codes main_imgbtn_camera
  ui->main_imgbtn_camera = lv_imgbtn_create(ui->main);
  lv_obj_set_pos(ui->main_imgbtn_camera, 20, 135);
  lv_obj_set_size(ui->main_imgbtn_camera, 80, 80);
  lv_img_set_src(ui->main_imgbtn_camera, &img_camera);
  lv_obj_add_style(ui->main_imgbtn_camera, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed

  //Write codes main_imgbtn_picture
  ui->main_imgbtn_picture = lv_imgbtn_create(ui->main);
  lv_obj_set_pos(ui->main_imgbtn_picture, 140, 135);
  lv_obj_set_size(ui->main_imgbtn_picture, 80, 80);
  lv_img_set_src(ui->main_imgbtn_picture, &img_picture);
  lv_obj_add_style(ui->main_imgbtn_picture, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed

  //Write codes main_imgbtn_music
  ui->main_imgbtn_music = lv_imgbtn_create(ui->main);
  lv_obj_set_pos(ui->main_imgbtn_music, 20, 225);
  lv_obj_set_size(ui->main_imgbtn_music, 80, 80);
  lv_img_set_src(ui->main_imgbtn_music, &img_music);
  lv_obj_add_style(ui->main_imgbtn_music, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed

  //Write codes main_imgbtn_heartrate
  ui->main_imgbtn_heartrate = lv_imgbtn_create(ui->main);
  lv_obj_set_pos(ui->main_imgbtn_heartrate, 140, 225);
  lv_obj_set_size(ui->main_imgbtn_heartrate, 80, 80);
  lv_img_set_src(ui->main_imgbtn_heartrate, &img_heartrate);
  lv_obj_add_style(ui->main_imgbtn_heartrate, &style_pr, LV_STATE_PRESSED);  //Triggered when the button is pressed

  lv_obj_add_event_cb(ui->main_imgbtn_logo, main_imgbtn_logo_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_camera, main_imgbtn_camera_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_picture, main_imgbtn_picture_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_music, main_imgbtn_music_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_heartrate, main_imgbtn_heartrate_event_handler, LV_EVENT_ALL, NULL);
}

//Load Ui Screen function
void setup_ui(lvgl_main_ui *ui) {
  setup_scr_main(ui);
  lv_scr_load(ui->main);
}
