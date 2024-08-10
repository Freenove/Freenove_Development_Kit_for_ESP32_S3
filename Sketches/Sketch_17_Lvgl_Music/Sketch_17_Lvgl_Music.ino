#include "display.h"
#include <lvgl.h>
#include "sd_card.h"
#include "music_ui.h"

Display screen;

void setup(){
    /* prepare for possible serial debug */
    Serial.begin( 115200 );

   /*** Init drivers ***/
    sdcard_init();       //Initialize the SD_MMC module
    screen.init();
    music_iis_init();

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );
    
    setup_scr_music(&guider_music_ui);
    lv_scr_load(guider_music_ui.music);

    Serial.println( "Setup done" );
}

void loop(){
    screen.routine(); /* let the GUI do its work */
    delay( 5 );
}
