#ifndef __SDCARD_H
#define __SDCARD_H

#include "Arduino.h"
#include "FS.h"
#include "SD_MMC.h"

#define SD_MMC_CMD 38    //Please do not modify it.
#define SD_MMC_CLK 39    //Please do not modify it.
#define SD_MMC_D0 40     //Please do not modify it.

#define MUSIC_FOLDER     "/music"
#define PICTURE_FOLDER   "/picture"
#define LOGO_FOLDER      "/logo"

#define FILE_NAME_LENGTH 255    //Filename length
#define AREA_SIZE (1024 * 1)  //The size of read/write area

typedef struct Link {
  int file_index;                    //Node index number
  char file_name[FILE_NAME_LENGTH];  //Node file name
  long file_size;                    //Node file size
  struct Link *file_prev;            //Points to the previous linked list node
  struct Link *file_next;            //Points to the next listed node
} list_link;                         //Define a structure named list_link

extern list_link *list_music;        //Defines a pointer that holds an audio link list
extern list_link *list_picture;      //Defines a pointer that holds an picture link list
extern list_link *list_logo;         //Defines a pointer that holds an logo link list

int sdcard_init(void);
void sdcard_deinit(void);

int sdcard_write(char *path, char *name, uint8_t *content, int size);
int sdcard_read_file(char *path, uint8_t *content, long size, SeekMode location = (SeekMode)(SeekSet), long offset = 0);
int sdcard_read_file_size(char *path);
int sdcard_is_exists(char *path);
int sdcard_create_dir(char *path);
int sdcard_remove_dir(char *path);
int sdcard_remeve_file(char *path);
void sdcard_list_dir(const char * dirname, uint8_t levels);

list_link *list_create_node(int index, char *name, long size);           //Create a linked list node
void list_insert_tail(list_link *phead, char *name, long size);          //Adds a node from the end of the linked list
list_link *sdcard_get_file_parameter(char *path, char *extension = "");  //将文件夹下指定类型的文件做成链表
list_link *list_destory(list_link *phead);                               //Delete the contents of the linked list
void list_printf(list_link *phead);                                      //Print the contents of the linked list
void list_printf_back(list_link *phead);                                 //Print the contents of the list in reverse order
char *list_find_node(list_link *phead, int index);                       //Query the index node of the linked list and return the name of the node
int list_count_number(list_link *phead);                                 //Calculate the number of linked list nodes

int sdcard_wifi_file_read(uint8_t *wifi_name, uint8_t *wifi_password, char *path="/router", char *name="WiFi.txt");   //读取文件中WiFi名称和密码
int sdcard_wifi_file_write(uint8_t *wifi_name, uint8_t *wifi_password, char *path="/router", char *name="WiFi.txt");  //写入WiFi文件中的WiFi名称和密码

void write_rgb565_to_bmp(char *path, uint8_t *buf, long size, long height, long width);
void setup_list_head_music(void);    //Generate a linked list of audio files
void setup_list_head_picture(void);  //Generate a linked list of image bin files
void setup_list_head_logo(void);     //Generate a linked list of logo jpg files
#endif
