#include <lvgl.h>
#include "Arduino.h"
#include "display.h"

Display screen;
void custom_lable(void);

void setup() {
  Serial.begin(115200);

  /*** Init screen ***/
  screen.init();

  /***  Print lvgl version ***/
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");
  Serial.println("Setup done");

  /*** The custom code ***/
  custom_lable();

}

void loop() {
  screen.routine();
  delay(5);
}

void custom_lable(void)
{
  lv_obj_t * label = lv_label_create(lv_scr_act());
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);      /*Break the long lines*/
  lv_label_set_text(label, "Welcome to Freenove.");
  lv_obj_set_width(label, 150);  /*Set smaller width to make the lines wrap*/
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
