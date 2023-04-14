#ifndef __CAMERA_H
#define __CAMERA_H

#include "Arduino.h"
#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"
#include "esp_camera.h"

int camera_init();//Initialize the camera drive
void camera_set_flip_vertical(bool state);//Flip Vertical
void camera_set_mirror_horizontal(bool state);//Mirror Horizontal
bool camera_get_flip_vertical(void);
bool camera_get_mirror_horizontal(void);

#endif