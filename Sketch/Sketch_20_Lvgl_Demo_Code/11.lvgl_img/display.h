#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "lvgl.h"

#ifndef I2C_SDA
#define I2C_SDA 2
#endif
#ifndef I2C_SCL
#define I2C_SCL 1
#endif
#define RST_N_PIN -1
#define INT_N_PIN -1

#define TFT_DIRECTION 0   //TFT direction

class Display
{
private:

public:
    void init();
    void routine();
};

#endif
