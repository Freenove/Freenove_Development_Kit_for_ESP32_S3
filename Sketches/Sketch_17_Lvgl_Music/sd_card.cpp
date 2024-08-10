#include "sd_card.h"
#include "FS.h"
#include "SD_MMC.h"

list_link *list_music=NULL;    //Defines a pointer that holds an audio link list
list_link *list_picture=NULL;  //Defines a pointer that holds an picture link list
list_link *list_logo=NULL;     //Defines a pointer that holds an logo link list

//Initialize the SD_MMC
int sdcard_init(void) {
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  int error = SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5);
  if (!error) {
    Serial.printf("Card Mount Failed: %d\r\n", error);
    return 0;
  }
  Serial.println("Card Mount Success.");
  return 1;
}

//Generate a linked list of audio files
void setup_list_head_music(void){
  create_folder(MUSIC_FOLDER);
  if(list_music!=NULL)
    list_music=list_destory(list_music);
  list_music=sdcard_read_folder(MUSIC_FOLDER,"mp3");
  list_printf(list_music);
}

//Generate a linked list of image bin files
void setup_list_head_picture(void){
  create_folder(PICTURE_FOLDER);
  if(list_picture!=NULL)
    list_picture=list_destory(list_picture);
  list_picture=sdcard_read_folder(PICTURE_FOLDER,"bmp");
  //list_printf(list_picture);
}

//Generate a linked list of logo jpg files
void setup_list_head_logo(void){
  create_folder(LOGO_FOLDER);
  if(list_logo!=NULL)
    list_logo=list_destory(list_logo);
  list_logo=sdcard_read_folder(LOGO_FOLDER,"jpg");
  //list_printf(list_logo);
}

//Writes anything to a file
void write_file(char *path, uint8_t *buf, long size){
  File file = SD_MMC.open(path, FILE_WRITE);
  if(!file){
      Serial.println("Failed to open file for writing");
      return;
  }
  long buffer = file.write(buf, size);
  if(buffer==size){
      Serial.println("File written");
  } else {
      Serial.println("Write failed");
  }
  file.close();
  //Serial.printf("Saved file to path: %s\r\n", path);
}

//Writes anything to the pointed file, save as bmp.
void write_rgb565_to_bmp(char *path, uint8_t *buf, long size, long height, long width){
  File file = SD_MMC.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  uint8_t bmp_info[]={
    0x42,0x4d,0x48,0xc2,0x01,0x00,0x00,0x00,0x00,0x00,0x46,0x00,0x00,0x00,0x38,0x00,
    0x00,0x00,0xf0,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x01,0x00,0x10,0x00,0x03,0x00,
    0x00,0x00,0x02,0xc2,0x01,0x00,0x12,0x0b,0x00,0x00,0x12,0x0b,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x00,0x00,0xe0,0x07,0x00,0x00,0x1f,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00
  };
  uint8_t bmp_efo[]={0x00,0x00};
  file.write(bmp_info, 70);
  for(int i=height-1;i>=0;i--){
    file.write(&buf[i*width*2], width*2);
  }
  file.write(bmp_efo, 2);
  file.close();
}

//If the folder does not exist, create a folder
void create_folder(char *path){
  File root = SD_MMC.open(path);
  if(!root){
      Serial.println("Failed to open directory");
      if(SD_MMC.mkdir(path)){
        Serial.println("Dir created");
      } else {
          Serial.println("mkdir failed");
      }
  }
  root.close();
}

//Delete the pointed file
void delete_file(char *path){
  String buf;
  buf.concat(path);
  buf.remove(0,2);
  Serial.println(buf);
  if(SD_MMC.remove((char *)buf.c_str())){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

//Reads the file name of the specified folder and generates and returns a linked list
list_link *sdcard_read_folder(char *dirname, char *extension){
  String str="";
  list_link *phead=list_create_node(0,dirname);  
  File root = SD_MMC.open(dirname);
  if(!root){
      Serial.println("Failed to open directory");
      return phead;
  }
  if(!root.isDirectory()){
      Serial.println("Not a directory");
      return phead;
  }
  File file = root.openNextFile();
  while(file){
    str.concat(file.name());
    if(str.endsWith(extension)==true){
          list_insert_tail(phead,(char *)str.c_str());
    }
    str="";
    file = root.openNextFile();
  }
  root.close();
  return phead;
}

//Create a linked list node
list_link *list_create_node(int index, char *name) {
  list_link *new_node = (list_link *)malloc(sizeof(list_link));
  if (new_node == NULL) {
    Serial.println("malloc node failed.");
  } else {
    new_node->index_number = index;
    memset(new_node->file_name, 0, FILE_NAME_LENGTH);  
    memcpy(new_node->file_name, name, strlen(name));   
    new_node->file_prev = NULL;
    new_node->file_next = NULL;
  }
  return new_node;
}

//Adds a node from the end of the linked list
void list_insert_tail(list_link *phead, char *name) {
  list_link *temp = phead;
  while (temp->file_next != NULL) {
    temp = temp->file_next;
  }
  list_link *new_node = list_create_node((temp->index_number + 1), name);  
  new_node->file_prev = temp;
  temp->file_next = new_node;
}

//Print the contents of the linked list
void list_printf(list_link *phead) {
  list_link *temp = phead;
  while (temp != NULL) {
    Serial.println(temp->file_name);
    temp = temp->file_next;
  }
}

//Delete the contents of the linked list
list_link *list_destory(list_link *phead) {
  list_link *temp;
  while (phead->file_next != NULL) {
    temp = phead->file_next;
    free(phead);
    phead = temp;
  }
  temp = phead->file_next;
  free(phead);
  phead = temp;
  return phead;
}

//Query the index node of the linked list and return the name of the node
char *list_find_node(list_link *phead, int index) {
  int list_max_num=list_count_number(phead);
  if (index <= 0 || index > list_max_num)
    return NULL;
  
  list_link *temp;
  temp = phead;
  while (temp != NULL) {
    if (temp->index_number != index)
      temp = temp->file_next;
    else
      break;
  }
  if (temp != NULL) {
    return temp->file_name;
  }
  return NULL;
}

//Print the contents of the list in reverse order
void list_printf_back(list_link *phead) {
  list_link *temp = phead;
  while (temp->file_next != NULL) {
    temp = temp->file_next;
  }
  while (temp->file_prev != NULL) {
    Serial.println(temp->file_name);
    temp = temp->file_prev;
  }
  Serial.println(temp->file_name);
}

//Calculate the number of linked list nodes
int list_count_number(list_link *phead){
  int num=0;
  list_link *temp;
  temp=phead;
  while(temp->file_next!=NULL){
    num++;
    temp=temp->file_next;
  }
  return num;
}
