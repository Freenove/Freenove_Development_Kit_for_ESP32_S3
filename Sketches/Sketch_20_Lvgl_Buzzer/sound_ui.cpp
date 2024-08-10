//#include "second_main_ui.h"
#include "sound_ui.h"
#include "lv_img.h"

lvgl_sound_ui guider_sound_ui;  //sound ui structure
int buzzer_state = 0;
int buzzer_time = 100;

static void lv_spinbox_increment_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_increment(guider_sound_ui.sound_spinbox_buzzer);
    buzzer_time = lv_spinbox_get_value(guider_sound_ui.sound_spinbox_buzzer);
    Serial.printf("data:%d\r\n", buzzer_time);
  }
}

static void lv_spinbox_decrement_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_decrement(guider_sound_ui.sound_spinbox_buzzer);
    buzzer_time = lv_spinbox_get_value(guider_sound_ui.sound_spinbox_buzzer);
    Serial.printf("data:%d\r\n", buzzer_time);
  }
}

static void sound_btnmatrix_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    if (buzzer_state == 1) {
      sound_set_buzzer(id + 1);
    } else {
      sound_set_buzzer(0);
    }
  }
}

static void sound_checkbox_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    buzzer_state = lv_obj_get_state(obj) & LV_STATE_CHECKED ? 1 : 0;
  }
}

static void sound_home_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the home button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        /*
        if (!lv_obj_is_valid(guider_second_main_ui.second_main))
          setup_scr_second_main(&guider_second_main_ui);
        lv_scr_load(guider_second_main_ui.second_main);
        lv_obj_del(guider_sound_ui.sound);
        */
      }
      break;
    default:
      break;
  }
}
//Parameter configuration function on the sound screen
void setup_scr_sound(lvgl_sound_ui *ui) {
  buzzer_init();

  //Write codes picture
  ui->sound = lv_obj_create(NULL);
  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->sound, &bg_style, LV_PART_MAIN);

  lv_img_home_init();
  //Write codes sound_imgbtn_home
  ui->sound_imgbtn_home = lv_imgbtn_create(ui->sound);
  lv_obj_remove_style_all(ui->sound_imgbtn_home);
  lv_obj_set_pos(ui->sound_imgbtn_home, 150, 10);
  lv_obj_set_size(ui->sound_imgbtn_home, 80, 80);
  lv_img_set_src(ui->sound_imgbtn_home, &img_home);
  static lv_style_t style_pr;
  lv_style_init(&style_pr);
  lv_style_set_translate_y(&style_pr, 5);
  lv_obj_add_style(ui->sound_imgbtn_home, &style_pr, LV_STATE_PRESSED);

  ui->sound_checkbox_buzzer = lv_checkbox_create(ui->sound);
  lv_checkbox_set_text(ui->sound_checkbox_buzzer, "Buzzer");
  lv_obj_set_pos(ui->sound_checkbox_buzzer, 25, 15);

  ui->sound_spinbox_buzzer = lv_spinbox_create(ui->sound);
  lv_spinbox_set_range(ui->sound_spinbox_buzzer, 40, 1000);
  lv_spinbox_set_digit_format(ui->sound_spinbox_buzzer, 4, 4);
  lv_spinbox_step_prev(ui->sound_spinbox_buzzer);
  lv_obj_set_width(ui->sound_spinbox_buzzer, 50);
  lv_obj_set_pos(ui->sound_spinbox_buzzer, 50, 50);
  lv_spinbox_set_value(ui->sound_spinbox_buzzer, buzzer_time);

  ui->sound_btn_increment = lv_btn_create(ui->sound);
  lv_obj_set_size(ui->sound_btn_increment, 25, 25);
  lv_obj_align_to(ui->sound_btn_increment, ui->sound_spinbox_buzzer, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
  lv_obj_set_style_bg_img_src(ui->sound_btn_increment, LV_SYMBOL_PLUS, 0);
  lv_obj_add_event_cb(ui->sound_btn_increment, lv_spinbox_increment_event_cb, LV_EVENT_ALL, NULL);

  ui->sound_btn_decrement = lv_btn_create(ui->sound);
  lv_obj_set_size(ui->sound_btn_decrement, 25, 25);
  lv_obj_align_to(ui->sound_btn_decrement, ui->sound_spinbox_buzzer, LV_ALIGN_OUT_LEFT_MID, -2, 0);
  lv_obj_set_style_bg_img_src(ui->sound_btn_decrement, LV_SYMBOL_MINUS, 0);
  lv_obj_add_event_cb(ui->sound_btn_decrement, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);

  static const char *btnm_map[] = { "do", "re", "mi", "\n", "fa", "so", "la", "\n", "si", "" };
  ui->sound_btnmatrix = lv_btnmatrix_create(ui->sound);
  lv_btnmatrix_set_map(ui->sound_btnmatrix, btnm_map);
  lv_obj_set_size(ui->sound_btnmatrix, 220, 200);
  lv_obj_set_pos(ui->sound_btnmatrix, 10, 110);
  lv_obj_add_event_cb(ui->sound_checkbox_buzzer, sound_checkbox_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->sound_btnmatrix, sound_btnmatrix_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->sound_imgbtn_home, sound_home_event_handler, LV_EVENT_ALL, NULL);
}

//Buzzer initialization
void buzzer_init(void) {
  pinMode(PIN_BUZZER, OUTPUT);
  ledcAttachChannel(PIN_BUZZER, BUZZER_FREQUENCY, 10, BUZZER_CHN);
  ledcWriteTone(PIN_BUZZER, 0);
}

void buzzer_alert(int freq, int time) {
  ledcWriteTone(PIN_BUZZER, freq * NOTE_BUZZER_GRADE);
  delay(time);
  ledcWriteTone(PIN_BUZZER, 0);
}

void buzzer_repeat_alert(int beat, int rebeat) {
  beat = constrain(beat, 1, 9);
  rebeat = constrain(rebeat, 1, 255);
  for (int j = 0; j < rebeat; j++) {
    for (int i = 0; i < beat; i++) {
      buzzer_alert(BUZZER_FREQUENCY, 100);
      delay(100);
    }
    delay(500);
  }
}

void sound_set_buzzer(int tone) {
  int freq = 0;
  switch (tone) {
    case 1: freq = NOTE_D1; break;
    case 2: freq = NOTE_D2; break;
    case 3: freq = NOTE_D3; break;
    case 4: freq = NOTE_D4; break;
    case 5: freq = NOTE_D5; break;
    case 6: freq = NOTE_D6; break;
    case 7: freq = NOTE_D7; break;
    default: freq = NOTE_D0; break;
  }
  buzzer_alert(freq, buzzer_time);
}
