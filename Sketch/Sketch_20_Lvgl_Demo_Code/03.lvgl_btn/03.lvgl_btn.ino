#include <lvgl.h>
#include "Arduino.h"
#include "display.h"
#include "lv_example_btn.h"

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
  //lv_example_btn_1();
  lv_example_btn_2();
  //lv_example_btn_3();

}

void loop() {
  screen.routine();
  delay(5);
}
