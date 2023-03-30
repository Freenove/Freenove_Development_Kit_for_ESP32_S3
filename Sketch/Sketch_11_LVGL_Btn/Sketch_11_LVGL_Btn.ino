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
  lv_example_btn_1();//Two types of button trigger events
  //lv_example_btn_2();//Style a button from scratch
  //lv_example_btn_3();//Create a style transition on a button to act like a gum when clicked

}

void loop() {
  screen.routine();
  delay(5);
}
