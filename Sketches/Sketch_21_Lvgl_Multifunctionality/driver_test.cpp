#include "driver_test.h"

#include "Audio.h"
#include "FS.h"
#include "Wire.h"

TaskHandle_t audioTaskHandle;  //audio test thread task handle

int index_line = 0;      //The line to which the record is displayed
unsigned char iic_address[10];    //Used to store the address scanned by the IIC

//Display the IIC address.
void tft_display_1(String buf, int data){
  String text;
  text = String(buf) + String(data, HEX);    
  tft.drawString((char *)text.c_str(), LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);//String, x, y, font_size
}

//Display the String.
int tft_display_2(int iic_num, int sd_init_state, int camera_init_state, int music_init_state){
  int state = 1;
  if(iic_num>=2){
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.drawString("Touch function init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
    tft.drawString("Max30102 init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  else if(iic_num>0){
    #if CHECK_SENSOR 
    state = 0;
    #endif
    if(iic_address[0]!=0x38){
      tft.setTextColor(TFT_RED, TFT_WHITE);
      tft.drawString("Please check the touch function.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2); delay(DELAY_TIME);
      tft.setTextColor(TFT_BLUE, TFT_WHITE);
      tft.drawString("Max30102 init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
    }
    else{
      tft.setTextColor(TFT_BLUE, TFT_WHITE);
      tft.drawString("Touch function init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
      tft.setTextColor(TFT_RED, TFT_WHITE);
      tft.drawString("Please check the max30102.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
    }
  }
  else{
    #if CHECK_SENSOR 
    state = 0;
    #endif
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.drawString("Please check the touch function.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2); delay(DELAY_TIME);
    tft.drawString("Please check the max30102.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  if(!sd_init_state){
    #if CHECK_SENSOR 
    state = 0;
    #endif
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.drawString("Please check the sd card.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  else{
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.drawString("SDcard init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  if(!camera_init_state){
    #if CHECK_SENSOR 
    state = 0;
    #endif
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.drawString("Please check the camera.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  else{
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.drawString("Camera init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  if(!music_init_state){
    #if CHECK_SENSOR 
    state = 0;
    #endif
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.drawString("Please check the IIS.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  else{
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.drawString("Music iis init success.", LEFT_LOCATION, ROW_HEIGHT *index_line++, 2);delay(DELAY_TIME);
  }
  #if CHECK_SENSOR 
  delay(1000);
  #endif
  return state;
}

//Initialize the IIC
void wire_init(void){
  Wire.begin(I2C_SDA,I2C_SCL);
}

//Scan the IIC
int wire_scan(void) {
  wire_init();
  unsigned char error, address;
  int nDevices = 0;
  for(address = 0x01; address < 0x7f; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      iic_address[nDevices++]=address;
    } 
  }
  return nDevices;
}

int driver_check_show(int iic_num, int sd_init_state, int camera_init_state, int music_init_state)
{
  tft.init();
  wire_init();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLUE, TFT_WHITE);
  for(int i=0;i<iic_num;i++)
  {
    tft_display_1("IIC address: 0x", iic_address[i]); 
    delay(DELAY_TIME);
  }
  index_line++;
  return tft_display_2(iic_num, sd_init_state, camera_init_state, music_init_state);
}

//music player thread
void loopTask_audio_test(void *pvParameters) {
  Serial.println("loopTask_audio_test start...");
  while (1) {
    music_loop();
    if(music_is_running()==0)
    {
      break;
    }
  }
  music_fs_end();
  vTaskDelete(audioTaskHandle);
}

void audio_init(void)
{
  music_fs_init();
  music_set_volume(21);
  music_load_fs_mp3("/test.mp3");
  xTaskCreate(loopTask_audio_test, "loopTask_audio_test", 8192, NULL, 1, &audioTaskHandle);
}

int audio_test_isRunning(void)
{
  return music_is_running();
}









