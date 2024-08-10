#include "display.h"
#include <lvgl.h>
#include "sd_card.h"
#include "camera.h"
#include "camera_ui.h"

Display screen;

void setup(){
    /* prepare for possible serial debug */
    Serial.begin( 115200 );
    
    /*** Init drivers ***/   
    sdcard_init();       //Initialize the SD_MMC module
    camera_init();       //Initialize the camera drive
    screen.init();       //Initialize the Screen

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );
    
    setup_scr_camera(&guider_camera_ui);//Initialize the camera UI
    lv_scr_load(guider_camera_ui.camera);//loading UI

    Serial.println( "Setup done" );
}

void loop(){
    screen.routine(); /* let the UI do its work */
    delay( 5 );
}
