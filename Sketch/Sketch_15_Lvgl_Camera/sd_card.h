#ifndef __SD_CARD_H
#define __SD_CARD_H

#include "Arduino.h"
#include "FS.h"
#include <LittleFS.h>
#include "SD_MMC.h"
#include "lvgl.h"
#include "stdlib.h"
#include "string.h"

#define SD_MMC_CMD 38                //Please do not modify it.
#define SD_MMC_CLK 39                //Please do not modify it. 
#define SD_MMC_D0  40                //Please do not modify it.

/*folder*/
#define MUSIC_FOLDER     "/music"
#define PICTURE_FOLDER   "/picture"
#define LOGO_FOLDER      "/logo"

#define FILE_NAME_LENGTH 255         //Filename length

typedef struct Link{
  int index_number;                  //Node index number
  char file_name[FILE_NAME_LENGTH];  //Node file name
  struct Link *file_prev;            //Points to the previous linked list node
  struct Link *file_next;            //Points to the next listed node
}list_link;                          //Define a structure named list_link

extern list_link *list_music;        //Defines a pointer that holds an audio link list
extern list_link *list_picture;      //Defines a pointer that holds an picture link list
extern list_link *list_logo;         //Defines a pointer that holds an logo link list

int sdcard_init(void);               //Initialize the SD_MMC

void setup_list_head_music(void);    //Generate a linked list of audio files
void setup_list_head_picture(void);  //Generate a linked list of image bin files
void setup_list_head_logo(void);     //Generate a linked list of logo jpg files

void write_file(char *path, uint8_t *buf, long size);//Writes anything to the pointed file
void write_rgb565_to_bmp(char *path, uint8_t *buf, long size, long height, long width);//Writes anything to the pointed file, save as bmp.
void create_folder(char *path);      //If the folder does not exist, create a folder
void delete_file(char *path);        //Delete the pointed file

list_link *sdcard_read_folder(char *dirname, char *extension);//Reads the file name of the specified folder and generates and returns a linked list
list_link *list_create_node(int index, char *name);           //Create a linked list node
void list_insert_tail(list_link *phead, char *name);          //Adds a node from the end of the linked list
void list_printf(list_link *phead);                           //Print the contents of the linked list
list_link *list_destory(list_link *phead);                    //Delete the contents of the linked list
char* list_find_node(list_link *phead, int index);            //Query the index node of the linked list and return the name of the node
void list_printf_back(list_link *phead);                      //Print the contents of the list in reverse order
int list_count_number(list_link *phead);                      //Calculate the number of linked list nodes


#endif