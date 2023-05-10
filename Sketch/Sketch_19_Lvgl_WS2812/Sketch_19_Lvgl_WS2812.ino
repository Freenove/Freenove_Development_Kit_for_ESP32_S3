#include <lvgl.h>
#include "Arduino.h"
#include "display.h"
#include "ws2812_ui.h"
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
  setup_scr_ws2812(&guider_ws2812_ui);
  lv_scr_load(guider_ws2812_ui.ws2812);
}

void loop() {
  screen.routine();
  delay(5);
}
