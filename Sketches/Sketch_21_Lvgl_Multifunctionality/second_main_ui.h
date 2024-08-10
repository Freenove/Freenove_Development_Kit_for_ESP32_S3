#ifndef __SECOND_MAIN_UI_H
#define __SECOND_MAIN_UI_H

#include "lvgl.h"
#include "Arduino.h"

typedef struct lvgl_second_main
{
	lv_obj_t *second_main;
	lv_obj_t *second_main_imgbtn_logo;
	lv_obj_t *second_main_imgbtn_ws2812;
	lv_obj_t *second_main_imgbtn_buzzer;
	lv_obj_t *second_main_imgbtn_folder;
	lv_obj_t *second_main_imgbtn_game;
}lvgl_second_main_ui;//second_main ui struct

extern lvgl_second_main_ui guider_second_main_ui;   //second_main ui structure 
void setup_scr_second_main(lvgl_second_main_ui *ui);//Parameter configuration function on the second_main screen
void setup_ui(lvgl_second_main_ui *ui);      //Load Ui Screen function

#endif
