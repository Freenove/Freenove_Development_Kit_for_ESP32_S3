#ifndef __WS2812_UI_H
#define __WS2812_UI_H

#include "lvgl.h"
#include "Arduino.h"

#define LEDS_COUNT 1
#define LEDS_PIN 48
#define CHANNEL 0

typedef struct lvgl_ws2812
{
	lv_obj_t *ws2812;
  lv_obj_t *ws2812_home;

  lv_obj_t *ws2812_slider_red;
  lv_obj_t *ws2812_slider_green;
  lv_obj_t *ws2812_slider_blue;
  lv_obj_t *ws2812_slider_brightness;

  lv_obj_t *ws2812_lable_red;
  lv_obj_t *ws2812_lable_green;
  lv_obj_t *ws2812_lable_blue;
  lv_obj_t *ws2812_lable_brightness;

  lv_obj_t *ws2812_text_red;
  lv_obj_t *ws2812_text_green;
  lv_obj_t *ws2812_text_blue;
  lv_obj_t *ws2812_text_brightness;

}lvgl_ws2812_ui;

extern lvgl_ws2812_ui guider_ws2812_ui;    //ws2812 ui structure 

bool ws2812_init(void);
void ws2812_set_color(int num, int red, int green, int blue, int brightness);

void setup_scr_ws2812(lvgl_ws2812_ui *ui); //Parameter configuration function on the ws2812 screen

#endif
