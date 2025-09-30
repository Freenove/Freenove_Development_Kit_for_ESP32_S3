#该类封装了一些调用esptool的指令参数
class COMMAND:
    #Firmware COMMAND
    PYTHON           = "python"
    PYTHON3          = "python3"
    SPACE            = " "
    ESPTOOL          = "./esptool/esptool.py"
    ERASE_FLASH      = "erase_flash"
    PORT             = "--port"
    BAUD             = "--baud"
    CHIP             = "--chip"
    ESP32            = "esp32S3"
    CONFIG           = "--before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 8MB"
    BOOTLOADER       = "0x0      ./firmware/bootloader.bin"
    PARTITION_TABLE  = "0x8000   ./firmware/partitions.bin"
    OTA_DATA_INITAL  = "0xe000   ./firmware/boot_app0.bin"
    FIRMWARE_CFG1    = "0x10000  ./firmware/firmware_cfg1.bin"
    FIRMWARE_CFG2    = "0x10000  ./firmware/firmware_cfg2.bin"
    MUSIC            = "0x310000 ./firmware/spiffs.bin"

    READ_MODE        = b"R"   #读数据模式
    WRITE_MODE       = b"W"   #写数据模式
    INTERVAL_CHAR    = b"#"   #分隔符
    DELETE_MODE      = b"D"   #删除模式
    LIST_MODE        = b"L"   #查询文件目录模式


def __init__(self):
    pass


