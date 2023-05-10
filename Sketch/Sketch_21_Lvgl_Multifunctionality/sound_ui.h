#ifndef __SOUND_UI_H
#define __SOUND_UI_H

#include "lvgl.h"
#include "Arduino.h"

#define NOTE_D0 0
#define NOTE_D1 174 
#define NOTE_D2 200 
#define NOTE_D3 226 
#define NOTE_D4 252 
#define NOTE_D5 276 
#define NOTE_D6 302 
#define NOTE_D7 328 
#define NOTE_BUZZER_GRADE 3

#define PIN_BUZZER 45            //Define the pins for the ESP32 control buzzer
#define BUZZER_CHN 0             //Define the PWM channel for ESP32
#define BUZZER_FREQUENCY 1000    //Define the resonant frequency of the buzzer 

typedef struct lvgl_sound
{
	lv_obj_t *sound;
	lv_obj_t *sound_imgbtn_home;
  lv_obj_t *sound_checkbox_buzzer;
  lv_obj_t *sound_spinbox_buzzer;
  lv_obj_t *sound_btn_decrement;
  lv_obj_t *sound_btn_increment;
  lv_obj_t *sound_btnmatrix;
}lvgl_sound_ui;

extern lvgl_sound_ui guider_sound_ui;//sound ui structure 

void buzzer_init(void);
void buzzer_alert(int freq, int time);
void buzzer_repeat_alert(int beat, int rebeat);
void sound_set_buzzer(int tone);

int sound_checkbox_switch(int buzzer_state, int audio_state);
void setup_scr_sound(lvgl_sound_ui *ui);//Parameter configuration function on the sound screen

#endif
