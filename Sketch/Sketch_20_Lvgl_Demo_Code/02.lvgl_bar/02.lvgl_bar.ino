#include <lvgl.h>
#include "Arduino.h"
#include "display.h"
#include "lv_example_bar.h"

Display screen;

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
  lv_example_bar_1();
  //lv_example_bar_2();
  //lv_example_bar_3();
  //lv_example_bar_4();
  //lv_example_bar_5();
}

void loop() {
  screen.routine();
  delay(5);
}
