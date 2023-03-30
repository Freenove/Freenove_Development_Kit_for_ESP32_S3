#ifndef __CAMERA_UI_H
#define __CAMERA_UI_H

#include "lvgl.h"
#include "Arduino.h"
#include "esp_camera.h"

extern camera_fb_t *fb;   

typedef struct lvgl_camera
{
	lv_obj_t *camera;
	lv_obj_t *camera_video;
	lv_obj_t *camera_imgbtn_photo;
	lv_obj_t *camera_imgbtn_home;
}lvgl_camera_ui;

extern lv_img_dsc_t photo_show;              //apply an lvgl image variable
extern lvgl_camera_ui guider_camera_ui;      //camera ui structure 

void create_camera_task(void);               //Create camera task thread
void stop_camera_task(void);                 //Close the camera thread
void loopTask_camera(void *pvParameters);    //camera task thread
void ui_set_photo_show(void);                //Initialize an lvgl image variable
void setup_scr_camera(lvgl_camera_ui *ui);   //Parameter configuration function on the camera screen

#endif
