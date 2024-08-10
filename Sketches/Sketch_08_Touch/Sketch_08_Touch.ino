#include "FT6336U.h"

#define I2C_SDA 2
#define I2C_SCL 1
#define RST_N_PIN -1
#define INT_N_PIN -1

FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN); 
FT6336U_TouchPointType tp; 
int state = 0;

void setup() {
    Serial.begin(115200); 
    ft6336u.begin(); 
    Serial.print("FT6336U Firmware Version: "); 
    Serial.println(ft6336u.read_firmware_id());  
    Serial.print("FT6336U Device Mode: "); 
    Serial.println(ft6336u.read_device_mode());  
}

void loop() {
    tp = ft6336u.scan(); 
    char tempString[128]; 
    sprintf(tempString, "FT6336U TD Count %d / TD1 (%d, %4d, %4d) / TD2 (%d, %4d, %4d)\r\n", tp.touch_count, tp.tp[0].status, tp.tp[0].x, tp.tp[0].y, tp.tp[1].status, tp.tp[1].x, tp.tp[1].y); 
    Serial.print(tempString); 
    delay(100);
}

