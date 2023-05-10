#ifndef __WIFI_UI_H
#define __WIFI_UI_H

#include "lvgl.h"
#include "Arduino.h"

#define WIFI_CMD_PORT 5000
#define WIFI_VIDEO_PORT 8000
#define COMMANDS_COUNT_MAX 8

typedef struct wifi_cammand_information {
  char command;   
  char path[256]; 
  char name[256];  
  long size;      
  long position;  
} wifi_parameter;

extern wifi_parameter wifi_info;

typedef struct lvgl_wifi {
  lv_obj_t *wifi;
  lv_obj_t *wifi_lable_name;         
  lv_obj_t *wifi_lable_password;      
  lv_obj_t *wifi_btn_connect;        
  lv_obj_t *wifi_lable_connect;     
  lv_obj_t *wifi_btn_save;          
  lv_obj_t *wifi_lable_home;        
  lv_obj_t *wifi_textarea_name;      
  lv_obj_t *wifi_textarea_password;  
  lv_obj_t *wifi_lable_local_ip;     
  lv_obj_t *wifi_textarea_local_ip;  
  lv_obj_t *wifi_lable_remote_ip;     
  lv_obj_t *wifi_textarea_remote_ip;  
  lv_obj_t *wifi_keyboard;            
} lvgl_wifi_ui;

extern lvgl_wifi_ui guider_wifi_ui;     //wifi ui structure
void setup_scr_wifi(lvgl_wifi_ui *ui);  //Parameter configuration function on the wifi screen

int wifi_config_file_is_exists(char *path = "/router/WiFi.txt");
int wifi_config_file_delete(char *path = "/router/WiFi.txt");
void wifi_config_save(void);

int wifi_connect(char *wifi_name, char *wifi_passward);
int wifi_is_connect(void);
void wifi_disconect(void);

void create_wifi_task(void);              //Create wifi task thread
void stop_wifi_task(void);                //Close the wifi thread
void loopTask_wifi(void *pvParameters);   //wifi thread
void create_video_task(void);             //Create video task thread
void stop_video_task(void);               //Close the video thread
void loopTask_video(void *pvParameters);  //video thread

void wifi_message_parse(String msg);
void wifi_ack(int state);
void wifi_send_massage(wifi_parameter wifi_info);

void wifi_read_file_from_sdcard(void);
void wifi_write_file_to_sdcard(void);
void wifi_delete_file_from_sdcard(void);
void wifi_read_list_from_sdcard(void);

#endif
