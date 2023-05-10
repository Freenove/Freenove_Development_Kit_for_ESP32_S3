#ifndef __HEARTRATE_UI_H
#define __HEARTRATE_UI_H

#include "Arduino.h"
#include "lvgl.h"

#define I2C_SDA 2
#define I2C_SCL 1

#define HEARTRATE_SERIAL 0     //Serial print the original ir value, average value and dynamic data.
#define CHART_LOW_LIMIT 0      //Lower limit of Chart
#define CHART_HIGH_LIMIT 2000  //Upper limit of Chart

typedef struct lvgl_heartrate   
{
	lv_obj_t *heartrate;
	lv_obj_t *heartrate_chart;
	lv_obj_t *heartrate_label;
	lv_obj_t *heartrate_home;
}lvgl_heartrate_ui;

extern lvgl_heartrate_ui guider_heartrate_ui;     //music ui structure 

int heartrate_init(void);                        //Initialization heartrate module
void heartrate_shutdown(void);                   //The heartrate module is enabled to enter low power mode
void heartrate_wake_up(void);                    //Wake up the heartrate module

void loopTask_heartrate(void *pvParameters);     //heartrate task thread
void create_heartrate_task(void);                //Create heartrate task thread
void stop_heartrate_task(void);                  //Close the heartrate thread

void setup_scr_heartrate(lvgl_heartrate_ui *ui);  //Parameter configuration function on the heartrate screen

#endif
