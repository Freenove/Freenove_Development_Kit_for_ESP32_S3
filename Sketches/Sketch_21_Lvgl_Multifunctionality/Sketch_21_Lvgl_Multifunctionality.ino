#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "display.h"
#include "FT6336U.h"
#include "main_ui.h"
#include "camera.h"
#include "sd_card.h"
#include "driver_test.h"
#include "sound_ui.h"

Display screen;
int setup_success = 1;

void setup() {
  /* prepare for possible serial debug */
  Serial.begin(115200);
  /*** Init drivers ***/
  int iic_num = wire_scan();
  int sd_init_state = sdcard_init();          //Initialize the SD_MMC module
  int camera_init_state = camera_init_jpg();  //Initialize the camera drive
  int music_init_state = music_iis_init();    //Initialize the audio interface
  buzzer_init();                              //Initialize the buzzer
  buzzer_repeat_alert(3,1);                   //Test the buzzer
  audio_init();                               //Test the audio module
  setup_success = driver_check_show(iic_num, sd_init_state, camera_init_state, music_init_state);

  if (setup_success != 0) {
    screen.init();
    music_set_volume(10);
    heartrate_init();

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println(LVGL_Arduino);
    Serial.println("I am LVGL_Arduino");

    setup_ui(&guider_main_ui);     //Load Ui Screen function: guider_main_ui
    Serial.println("Setup done");  //Print prompt message
  }
  while (audio_test_isRunning());  //Wait for the audio to end.
}

void loop() {
  if (setup_success != 0) {
    screen.routine(); /* let the GUI do its work */
    delay(5);
  }
}
