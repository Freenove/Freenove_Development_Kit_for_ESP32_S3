#ifndef __DRIVER_TEST_H
#define __DRIVER_TEST_H

#include "TFT_eSPI.h"

#define CHECK_SENSOR 1

#ifndef I2C_SDA
#define I2C_SDA 2
#endif
#ifndef I2C_SCL
#define I2C_SCL 1
#endif

#define DELAY_TIME   100  //Time for printing detection information
#define ROW_HEIGHT    20  //Height of each row
#define LEFT_LOCATION 10  //Distance from the left boundary

#define PIN_BUZZER 45                   //Define the pins for the ESP32 control buzzer
#define BUZZER_CHN 0                    //Define the PWM channel for ESP32
#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 

extern TFT_eSPI tft;

void wire_init(void);
int wire_scan(void);
int driver_check_show(int iic_num, int sd_init_state, int camera_init_state, int music_init_state);
void audio_init(void);
int audio_test_isRunning(void);
void buzzer_init(void);
void buzzer_alert(int beat, int rebeat);

#endif
