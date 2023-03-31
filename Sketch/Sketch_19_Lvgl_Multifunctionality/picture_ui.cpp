#include "picture_ui.h"
#include "sd_card.h"
#include "main_ui.h"
#include "lv_img.h"

lvgl_picture_ui guider_picture_ui;  //picture ui structure
static int picture_index_num = 1;   //index number of the picture

//Click the left icon, callback function: show the last picture
static void picture_imgbtn_left_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    Serial.println("Clicked the left button.");
    picture_index_num--;
    if (picture_index_num < 1)
      picture_index_num = list_count_number(list_picture);
    picture_imgbtn_display(list_find_node(list_picture, picture_index_num));
  }
}

//Click the right icon, callback function: show the next picture
static void picture_imgbtn_right_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    Serial.println("Clicked the right button.");
    picture_index_num++;
    if (picture_index_num > list_count_number(list_picture))
      picture_index_num = 1;
    picture_imgbtn_display(list_find_node(list_picture, picture_index_num));
  }
}

//Click the logo icon, callback function: goes to the main ui interface
static void picture_imgbtn_home_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the return button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        if (!lv_obj_is_valid(guider_main_ui.main))
          setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_picture_ui.picture);
      }
      break;
    default:
      break;
  }
}

//Parameter configuration function on the picture screen
void setup_scr_picture(lvgl_picture_ui *ui) {
  //Write codes picture
  ui->picture = lv_obj_create(NULL);

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->picture, &bg_style, LV_PART_MAIN);

  lv_img_home_init();
  lv_img_left_init();
  lv_img_right_init();

  /*Init the pressed style*/
  static lv_style_t style_pr;//Apply for a style
  lv_style_init(&style_pr);  //Initialize it

  lv_style_set_translate_y(&style_pr, 5);//Style: Every time you trigger, move down 5 pixels

  //Write codes picture_left
  ui->picture_left = lv_imgbtn_create(ui->picture);
  lv_obj_set_pos(ui->picture_left, 10, 250);
  lv_obj_set_size(ui->picture_left, 60, 60);
  lv_img_set_src(ui->picture_left, &img_left);
  lv_obj_add_style(ui->picture_left, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  //Write codes picture_right
  ui->picture_right = lv_imgbtn_create(ui->picture);
  lv_obj_set_pos(ui->picture_right, 170, 250);
  lv_obj_set_size(ui->picture_right, 60, 60);
  lv_img_set_src(ui->picture_right, &img_right);
  lv_obj_add_style(ui->picture_right, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  //Write codes picture_home
  ui->picture_home = lv_imgbtn_create(ui->picture);
  lv_obj_remove_style_all(ui->picture_home);
  lv_obj_set_pos(ui->picture_home, 80, 240);
  lv_obj_set_size(ui->picture_home, 80, 80);
  lv_img_set_src(ui->picture_home, &img_home);
  lv_obj_add_style(ui->picture_home, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  //Write codes picture_show
  ui->picture_show = lv_img_create(ui->picture);
  lv_obj_set_pos(ui->picture_show, 0, 0);
  lv_obj_set_size(ui->picture_show, 240, 240);

  setup_list_head_picture();
  picture_index_num = list_count_number(list_picture);
  picture_imgbtn_display(list_find_node(list_picture, picture_index_num));

  lv_obj_add_event_cb(ui->picture_left, picture_imgbtn_left_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->picture_right, picture_imgbtn_right_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->picture_home, picture_imgbtn_home_event_handler, LV_EVENT_ALL, NULL);
}

//Read the image file and display it
void picture_imgbtn_display(char *name) {
  if (name != NULL) {
    char buf_picture_name[100] = {"S:"};
    strcat(buf_picture_name, PICTURE_FOLDER);
    strcat(buf_picture_name, "/");
    strcat(buf_picture_name, name);
    lv_img_set_src(guider_picture_ui.picture_show, buf_picture_name);
  }
  else {
    lv_img_set_src(guider_picture_ui.picture_show, LV_SYMBOL_DUMMY "The picture folder has no files.");
  }
}
