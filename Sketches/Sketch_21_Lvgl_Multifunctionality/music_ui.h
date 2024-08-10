#ifndef __MUSIC_H
#define __MUSIC_H

#include "lvgl.h"
#include "Arduino.h"

#define I2S_BCLK   42
#define I2S_DOUT   41
#define I2S_LRC    14

typedef struct lvgl_music
{
	lv_obj_t *music;
	lv_obj_t *music_imgbtn_home;
	lv_obj_t *music_imgbtn_left;
	lv_obj_t *music_imgbtn_play;
  lv_obj_t *music_imgbtn_stop;
	lv_obj_t *music_imgbtn_right;
	lv_obj_t *music_label;
  lv_obj_t *music_slider_valume;
  lv_obj_t *music_slider_label;
  lv_obj_t *music_bar_time;
}lvgl_music_ui;

extern lvgl_music_ui guider_music_ui;//music ui structure 
void setup_scr_music(lvgl_music_ui *ui);//Parameter configuration function on the music screen

void music_set_label_text(char *text);     //Set the label display content
int music_iis_init(void);                  //Initialize the audio interface
void music_set_volume(int volume);         //Set the volume: 0-21
int music_read_volume(void);               //Query volume
void music_load_mp3(char* name);           //load the mp3
void music_pause_resume(void);             //Pause/play the music
void music_stop(void);                     //Stop the music
bool music_is_running(void);               //Whether the music is running
long music_get_total_playing_time(void);   //Gets how long the music player has been playing
long music_get_file_duration(void);        //Obtain the playing time of the music file
bool music_set_play_position(int second);  //Set play position
long music_read_play_position(void);       //Gets the current playing time of the music
void music_loop(void);                     //Non-blocking audio execution function
void music_fs_init(void);                  //Init SPIFS
void music_fs_end(void);                   //Close SPIFS
void music_load_fs_mp3(char *name);        //load FS mp3
void loopTask_music(void *pvParameters);   //music player task thread
void start_music_task(void);               //Create music task thread
void stop_music_task(void);                //Close the music thread
int music_task_is_running(void);           //Check whether the thread is running
//optional
void audio_info(const char *info);         
void audio_eof_mp3(const char *info);      









#endif
