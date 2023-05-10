#include <lvgl.h>
#include "Arduino.h"
#include "display.h"
#include "sound_ui.h"

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
  setup_scr_sound(&guider_sound_ui);
  lv_scr_load(guider_sound_ui.sound);
}

void loop() {
  screen.routine();
  delay(5);
}
