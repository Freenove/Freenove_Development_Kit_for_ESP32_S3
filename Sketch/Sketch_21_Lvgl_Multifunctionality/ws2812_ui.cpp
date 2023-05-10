#include "second_main_ui.h"
#include "lvgl.h"
#include "ws2812_ui.h"
#include "lv_img.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"

lvgl_ws2812_ui guider_ws2812_ui;
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

bool ws2812_init(void) {
  strip.begin();
  strip.setBrightness(1);
  return 1;
}

void ws2812_set_color(int num, int red, int green, int blue, int brightness){
  red = constrain(red, 0, 255);
  green = constrain(green, 0, 255);
  blue = constrain(blue, 0, 255);
  brightness = constrain(brightness, 0, 255);
  strip.setBrightness(brightness);
  strip.setLedColorData(num, red, green, blue);  
  strip.show();
}

static void ws2812_imgbtn_home_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the home button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        strip.setLedColorData(0, 0, 0, 0);  
        strip.setBrightness(0);
        strip.show();
        if (!lv_obj_is_valid(guider_second_main_ui.second_main))
          setup_scr_second_main(&guider_second_main_ui);
        lv_scr_load(guider_second_main_ui.second_main);
        lv_obj_del(guider_ws2812_ui.ws2812);
      }
      break;
    default:
      break;
  }
}

static void slider_event_cb(lv_event_t *e) {
  LV_UNUSED(e);
  int color_red = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_red);
  int color_green = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_green);
  int color_blue = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_blue);
  int color_brightness = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_brightness);

  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_text_red, "%d", color_red);
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_text_green, "%d", color_green);
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_text_blue, "%d", color_blue);
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_text_brightness, "%d", color_brightness);

  ws2812_set_color(0, color_red, color_green, color_blue, color_brightness);
}

void create_lable(lvgl_ws2812_ui *ui, lv_obj_t **lable1, lv_obj_t **lable2) {
  *lable1 = lv_label_create(ui->ws2812);
  lv_obj_set_style_text_align(*lable1, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_set_size(*lable1, 85, 20);

  *lable2 = lv_label_create(ui->ws2812);
  lv_obj_set_style_text_align(*lable2, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_size(*lable2, 45, 20);
}

static lv_obj_t *create_slider(lvgl_ws2812_ui *ui, lv_color_t color) {
  lv_obj_t *slider = lv_slider_create(ui->ws2812);
  lv_slider_set_range(slider, 0, 255);
  lv_obj_set_size(slider, 200, 10);
  lv_obj_set_style_bg_color(slider, color, LV_PART_KNOB);
  lv_obj_set_style_bg_color(slider, color, LV_PART_INDICATOR);
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  return slider;
}

//Parameter configuration function on the ws2812 screen
void setup_scr_ws2812(lvgl_ws2812_ui *ui) {
  ws2812_init();

  //Write codes picture
  ui->ws2812 = lv_obj_create(NULL);
  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->ws2812, &bg_style, LV_PART_MAIN);

  lv_img_home_init();
  //Write codes picture_home
  ui->ws2812_home = lv_imgbtn_create(ui->ws2812);
  lv_obj_remove_style_all(ui->ws2812_home);
  lv_obj_set_pos(ui->ws2812_home, 150, 10);
  lv_obj_set_size(ui->ws2812_home, 80, 80);
  lv_img_set_src(ui->ws2812_home, &img_home);
  static lv_style_t style_pr;              //Apply for a style
  lv_style_init(&style_pr);                //Initialize it
  lv_style_set_translate_y(&style_pr, 5);  //Style: Every time you trigger, move down 5 pixels
  lv_obj_add_style(ui->ws2812_home, &style_pr, LV_STATE_PRESSED);

  create_lable(ui, &ui->ws2812_lable_red, &ui->ws2812_text_red);
  lv_obj_set_pos(ui->ws2812_lable_red, 10, 10);
  lv_obj_set_pos(ui->ws2812_text_red, 95, 10);
  lv_label_set_text(ui->ws2812_lable_red, "red");
  lv_label_set_text(ui->ws2812_text_red, "255");
  static lv_style_t style_color_red;
  lv_style_init(&style_color_red);
  lv_style_set_border_width(&style_color_red, 2);
  lv_style_set_border_color(&style_color_red, lv_palette_main(LV_PALETTE_RED));
  lv_style_set_text_color(&style_color_red, lv_palette_main(LV_PALETTE_RED));
  lv_obj_add_style(ui->ws2812_lable_red, &style_color_red, LV_PART_MAIN);
  lv_obj_add_style(ui->ws2812_text_red, &style_color_red, LV_PART_MAIN);

  create_lable(ui, &ui->ws2812_lable_green, &ui->ws2812_text_green);
  lv_obj_set_pos(ui->ws2812_lable_green, 10, 30);
  lv_obj_set_pos(ui->ws2812_text_green, 95, 30);
  lv_label_set_text(ui->ws2812_lable_green, "green");
  lv_label_set_text(ui->ws2812_text_green, "255");
  static lv_style_t style_color_green;
  lv_style_init(&style_color_green);
  lv_style_set_border_width(&style_color_green, 2);
  lv_style_set_border_color(&style_color_green, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_text_color(&style_color_green, lv_palette_main(LV_PALETTE_GREEN));
  lv_obj_add_style(ui->ws2812_lable_green, &style_color_green, LV_PART_MAIN);
  lv_obj_add_style(ui->ws2812_text_green, &style_color_green, LV_PART_MAIN);

  create_lable(ui, &ui->ws2812_lable_blue, &ui->ws2812_text_blue);
  lv_obj_set_pos(ui->ws2812_lable_blue, 10, 50);
  lv_obj_set_pos(ui->ws2812_text_blue, 95, 50);
  lv_label_set_text(ui->ws2812_lable_blue, "blue");
  lv_label_set_text(ui->ws2812_text_blue, "255");
  static lv_style_t style_color_blue;
  lv_style_init(&style_color_blue);
  lv_style_set_border_width(&style_color_blue, 2);
  lv_style_set_border_color(&style_color_blue, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_text_color(&style_color_blue, lv_palette_main(LV_PALETTE_BLUE));
  lv_obj_add_style(ui->ws2812_lable_blue, &style_color_blue, LV_PART_MAIN);
  lv_obj_add_style(ui->ws2812_text_blue, &style_color_blue, LV_PART_MAIN);

  create_lable(ui, &ui->ws2812_lable_brightness, &ui->ws2812_text_brightness);
  lv_obj_set_pos(ui->ws2812_lable_brightness, 10, 70);
  lv_obj_set_pos(ui->ws2812_text_brightness, 95, 70);
  lv_label_set_text(ui->ws2812_lable_brightness, "brightness");
  lv_label_set_text(ui->ws2812_text_brightness, "255");
  static lv_style_t style_color_brightness;
  lv_style_init(&style_color_brightness);
  lv_style_set_border_width(&style_color_brightness, 1);
  lv_style_set_border_color(&style_color_brightness, lv_palette_darken(LV_PALETTE_GREY,2));
  lv_style_set_text_color(&style_color_brightness, lv_palette_darken(LV_PALETTE_GREY,2));
  lv_obj_add_style(ui->ws2812_lable_brightness, &style_color_brightness, LV_PART_MAIN);
  lv_obj_add_style(ui->ws2812_text_brightness, &style_color_brightness, LV_PART_MAIN);

  ui->ws2812_slider_red = create_slider(ui, lv_palette_main(LV_PALETTE_RED));
  ui->ws2812_slider_green = create_slider(ui, lv_palette_main(LV_PALETTE_GREEN));
  ui->ws2812_slider_blue = create_slider(ui, lv_palette_main(LV_PALETTE_BLUE));
  ui->ws2812_slider_brightness = create_slider(ui, lv_palette_main(LV_PALETTE_GREY));

  lv_slider_set_value(ui->ws2812_slider_red, LV_OPA_10, LV_ANIM_OFF);
  lv_slider_set_value(ui->ws2812_slider_green, LV_OPA_10, LV_ANIM_OFF);
  lv_slider_set_value(ui->ws2812_slider_blue, LV_OPA_10, LV_ANIM_OFF);
  lv_slider_set_value(ui->ws2812_slider_brightness, LV_OPA_10, LV_ANIM_OFF);

  lv_obj_align(ui->ws2812_slider_red, LV_ALIGN_LEFT_MID, 20, -20);
  lv_obj_align_to(ui->ws2812_slider_green, ui->ws2812_slider_red, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
  lv_obj_align_to(ui->ws2812_slider_blue, ui->ws2812_slider_green, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
  lv_obj_align_to(ui->ws2812_slider_brightness, ui->ws2812_slider_blue, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
  
  lv_event_send(ui->ws2812_slider_brightness, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ui->ws2812_home, ws2812_imgbtn_home_event_handler, LV_EVENT_ALL, NULL);
}
