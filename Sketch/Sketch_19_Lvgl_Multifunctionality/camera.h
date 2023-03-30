#ifndef __CAMERA_H
#define __CAMERA_H

#include "Arduino.h"
#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"
#include "esp_camera.h"

int camera_init();//Initialize the camera drive

#endif