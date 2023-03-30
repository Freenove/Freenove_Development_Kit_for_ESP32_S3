#ifndef __MAIN_UI_H
#define __MAIN_UI_H

#include "lvgl.h"
#include "Arduino.h"

#include "camera_ui.h"
#include "picture_ui.h"
#include "music_ui.h"
#include "heartrate_ui.h"

typedef struct lvgl_main
{
	lv_obj_t *main;
	lv_obj_t *main_imgbtn_logo;
	lv_obj_t *main_imgbtn_camera;
	lv_obj_t *main_imgbtn_picture;
	lv_obj_t *main_imgbtn_music;
	lv_obj_t *main_imgbtn_heartrate;
}lvgl_main_ui;//main ui struct

extern lvgl_main_ui guider_main_ui;   //main ui structure 
void setup_scr_main(lvgl_main_ui *ui);//Parameter configuration function on the main screen
void setup_ui(lvgl_main_ui *ui);      //Load Ui Screen function

#endif
