#include "sd_card.h"
#include "stdlib.h"
#include "string.h"

list_link *list_music=NULL;    //Defines a pointer that holds an audio link list
list_link *list_picture=NULL;  //Defines a pointer that holds an picture link list
list_link *list_logo=NULL;     //Defines a pointer that holds an logo link list

int sdcard_init(void) {
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 50)) {
    return 0;
  }
  return 1;
}

void sdcard_deinit(void) {
  SD_MMC.end();
}

int sdcard_write_file(char *path, uint8_t *content, int size) {
  File file = SD_MMC.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return 0;
  }
  file.write(content, size);
  file.close();
  return 1;
}

int sdcard_append_file(char *path, char *name, uint8_t *content, int size) {
  char buf[FILE_NAME_LENGTH] = { 0 };
  strcat(buf, path);
  strcat(buf, "/");
  strcat(buf, name);
  File file = SD_MMC.open(buf, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return 0;
  }
  file.write(content, size);
  file.close();
  return 1;
}

int sdcard_write(char *path, char *name, uint8_t *content, int size) {
  int is_exists = sdcard_is_exists(path);
  if (!is_exists) {
    int create_state = sdcard_create_dir(path);
    if (!create_state) {
      Serial.printf("Dir:%s create failed, please check dir.\r\n", path);
      return 0;
    }
  }
  char buf[FILE_NAME_LENGTH] = { 0 };
  strcat(buf, path);
  strcat(buf, "/");
  strcat(buf, name);
  is_exists = sdcard_is_exists(buf);
  if (!is_exists) {
    Serial.printf("file is not exist. file:%s\r\n", buf);
    int write_state = sdcard_write_file(buf, content, size); 
    if (!write_state) {
      Serial.printf("File write failure\r\n"); 
      return 0;
    }
  } else {
    int write_state = sdcard_append_file(path, name, content, size);
    if (!write_state) {
      Serial.printf("File append failure\r\n"); 
      return 0;
    }
  }
  return 1;
}

int sdcard_read_file(char *path, uint8_t *content, long size, SeekMode location, long offset) {
  File file = SD_MMC.open(path, "rb");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return 0;
  }
  file.seek(offset, location);
  for (long i = 0; i < size; i++) {
    content[i] = file.read();
  }
  file.close();
  return 1;
}

int sdcard_read_file_size(char *path) {
  File file = SD_MMC.open(path, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return 0;
  }
  int file_size = file.size();
  file.close();
  return file_size;
}

int sdcard_is_exists(char *path) {
  int state = SD_MMC.exists(path);
  return state;
}

int sdcard_create_dir(char *path) {
  int state = SD_MMC.mkdir(path);
  return state;
}

int sdcard_remove_dir(char *path) {
  int state = SD_MMC.rmdir(path);
  return state;
}

int sdcard_remeve_file(char *path) {
  int state = SD_MMC.remove(path);
  return state;
}

void sdcard_list_dir(const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);
    File root = SD_MMC.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                sdcard_list_dir(file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

/*************************************************************************************/

//Create a linked list node
list_link *list_create_node(int index, char *name, long size) {
  list_link *new_node = (list_link *)malloc(sizeof(list_link));
  if (new_node == NULL) {
    Serial.println("malloc node failed.");
  } else {
    new_node->file_index = index;
    new_node->file_size = size;
    memset(new_node->file_name, 0, FILE_NAME_LENGTH);
    memcpy(new_node->file_name, name, strlen(name));
    new_node->file_prev = NULL;
    new_node->file_next = NULL;
  }
  return new_node;
}

//Adds a node from the end of the linked list
void list_insert_tail(list_link *phead, char *name, long size) {
  list_link *temp = phead;
  while (temp->file_next != NULL) {
    temp = temp->file_next;
  }
  list_link *new_node = list_create_node((temp->file_index + 1), name, size);
  new_node->file_prev = temp;
  temp->file_next = new_node;
}

//Reads the file name of the specified folder and generates and returns a linked list
list_link *sdcard_get_file_parameter(char *path, char *extension) {
  String str = "";
  list_link *phead = list_create_node(0, path, 0);
  File root = SD_MMC.open(path);
  if (!root) {
    Serial.println("Failed to open directory");
    return phead;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return phead;
  }
  File file = root.openNextFile();
  while (file) {
    str.concat(file.name());
    if (strncmp(extension, "\0", 1) == 0) {
      list_insert_tail(phead, (char *)str.c_str(), file.size());
    } else if (str.endsWith(extension) == true) {
      list_insert_tail(phead, (char *)str.c_str(), file.size());
    }
    str = "";
    file = root.openNextFile();
  }
  root.close();
  return phead;
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

//Print the contents of the linked list
void list_printf(list_link *phead) {
  list_link *temp = phead;
  if (temp == NULL) {
    Serial.println("phead is NULL.");
    return;
  }
  while (temp != NULL) {
    Serial.printf("%s:%d\r\n", temp->file_name, temp->file_size);
    temp = temp->file_next;
  }
}

//Print the contents of the list in reverse order
void list_printf_back(list_link *phead) {
  list_link *temp = phead;
  if (temp == NULL) {
    Serial.println("phead is NULL.");
    return;
  }
  while (temp->file_next != NULL) {
    temp = temp->file_next;
  }
  while (temp->file_prev != NULL) {
    Serial.printf("%s:%d\r\n", temp->file_name, temp->file_size);
    temp = temp->file_prev;
  }
  Serial.printf("%s:%d\r\n", temp->file_name, temp->file_size);
}


//Query the index node of the linked list and return the name of the node
char *list_find_node(list_link *phead, int index) {
  int list_max_num = list_count_number(phead);
  if (index <= 0 || index > list_max_num)
    return NULL;

  list_link *temp;
  temp = phead;
  while (temp != NULL) {
    if (temp->file_index != index)
      temp = temp->file_next;
    else
      break;
  }
  if (temp != NULL) {
    return temp->file_name;
  }
  return NULL;
}

//Calculate the number of linked list nodes
int list_count_number(list_link *phead) {
  int num = 0;
  list_link *temp = phead;
  if (temp == NULL) {
    Serial.println("phead is NULL.");
    return 0;
  }
  while (temp->file_next != NULL) {
    num++;
    temp = temp->file_next;
  }
  return num;
}

/*************************************************************************************/
int sdcard_wifi_file_read(uint8_t *wifi_name, uint8_t *wifi_password, char *path, char *name) {
  char buf[FILE_NAME_LENGTH] = { 0 };
  strcat(buf, path);
  strcat(buf, "/");
  strcat(buf, name);
  int count = sdcard_read_file_size(buf);
  uint8_t *buf_wifi_data = (uint8_t *)malloc(count + 1);
  int is_read_success = sdcard_read_file(buf, buf_wifi_data, count);
  if (is_read_success) {
    buf_wifi_data[count] = '\0';
    int i = 0;
    int j = 0;
    for (i = 0, j = 0; i < count; i++, j++) {
      if (buf_wifi_data[i] != '#')
        wifi_name[j] = buf_wifi_data[i];
      else
        break;
    }
    wifi_name[j] = '\0';
    for (i += 1, j = 0; i < count; i++, j++) {
      wifi_password[j] = buf_wifi_data[i];
    }
    wifi_password[j] = '\0';
  } else {
    Serial.println("wifi.txt read failed.");
    return 0;
  }
  return 1;
}

int sdcard_wifi_file_write(uint8_t *wifi_name, uint8_t *wifi_password, char *path, char *name) {
  if(!sdcard_is_exists(path))
    sdcard_create_dir(path);
  char buf[FILE_NAME_LENGTH] = { 0 };
  strcat(buf, path);
  strcat(buf, "/");
  strcat(buf, name);
  sdcard_remeve_file(buf);
  File file = SD_MMC.open(buf, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return 0;
  }
  file.write(wifi_name, strlen((char *)wifi_name));
  file.write((uint8_t*)("#"),1);      
  file.write(wifi_password, strlen((char *)wifi_password));
  file.close();
  return 1;
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

//Generate a linked list of audio files
void setup_list_head_music(void){
  sdcard_create_dir(MUSIC_FOLDER);
  if(list_music!=NULL)
    list_music=list_destory(list_music);
  list_music=sdcard_get_file_parameter(MUSIC_FOLDER,"mp3");
  list_printf(list_music);
}

//Generate a linked list of image bin files
void setup_list_head_picture(void){
  sdcard_create_dir(PICTURE_FOLDER);
  if(list_picture!=NULL)
    list_picture=list_destory(list_picture);
  list_picture=sdcard_get_file_parameter(PICTURE_FOLDER,"bmp");
  list_printf(list_picture);
}

//Generate a linked list of logo jpg files
void setup_list_head_logo(void){
  sdcard_create_dir(LOGO_FOLDER);
  if(list_logo!=NULL)
    list_logo=list_destory(list_logo);
  list_logo=sdcard_get_file_parameter(LOGO_FOLDER,"jpg");
  list_printf(list_logo);
}
