#include <lvgl.h>
#include "Arduino.h"
#include "display.h"
#include "lv_example_img.h"

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
  //lv_example_img_1();//Basic use method.
  //lv_example_img_2();//Demonstrate runtime image re-coloring
  lv_example_img_3();//Show transformations (zoom and rotation) using a pivot point.
}

void loop() {
  screen.routine();
  delay(5);
}
