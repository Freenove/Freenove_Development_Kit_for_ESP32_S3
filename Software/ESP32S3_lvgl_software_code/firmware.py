#!/usr/bin/python3
# -*- coding:utf-8 -*-
# @Time: 2023/04/20
# @Author: 林镇涛
# @Company: Freenove

import os
import sys                                 #添加系统传参对象
import serial                              #添加系统串口配置对象
import serial.tools.list_ports             #添加串口工具表对象
from PyQt5 import QtWidgets,QtCore         #添加窗口对象
from PyQt5.QtGui import QIcon              #添加图标对象
import platform                            #识别系统平台
from Thread import *
import subprocess

from ui.ui_firmware import Ui_widget_firmware #导入串口界面对象
from Command import COMMAND                #导入esptool指令参数

class ESP32S3_Firmware(QtWidgets.QWidget, Ui_widget_firmware):
    pyserial_chn = QtCore.pyqtSignal(int)
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        #添加界面的logo
        self.setWindowIcon(QIcon('./image/Freenove.ico'))                     #添加窗口logo

        #变量申请
        self.pyserial=serial.Serial()                                         #串口对象
        self.Com_Dict = {}                                                    #检测所有存在的串口，将信息存储在字典中
        self.cmd = COMMAND()                                                  #命令对象

        # 设置信号与槽函数
        self.connect()

    def print_information(self):
        print('The operating system is: ' + platform.system())
        print("1. Please connect ESP32 to your computer.")
        print("2. Click Scan Serial to find the Serial port.")
        print("3. Select the serial port corresponding to ESP32.")
        print("4. Select baud rate, the download speed is determined by baud rate, too high baud rate may fail.")
        print("5. Click the download button and wait for the download to complete.")

    #信号与槽部分
    def connect(self):
        self.button_firmware_scan.clicked.connect(self.port_check)                   #检测串口并重新更新到串口下拉框中
        self.combobox_firmware_port.currentTextChanged.connect(self.port_select)     #每次检测到串口发生变化
        self.combobox_firmware_baud.currentTextChanged.connect(self.baud_select)     #每次检测到波特率发生变化
        self.button_firmware_download.clicked.connect(self.download)                 #调用esptool上传固件
        self.pyserial_chn.connect(self.progressBar_show)                             #显示下载进度

    #串口检测
    def port_check(self):
        port_list = list(serial.tools.list_ports.comports())                #获取串口端口号并强制转化为列表并存入port_list
        self.combobox_firmware_port.clear()                                 #清除串口端口列表数据
        self.Com_Dict.clear()
        for port in port_list:                                              #解析串口列表数据，将其存入字典Com_Dict，并添加到屏幕端口号的下拉框中
            if port[0] != 'COM1':
                self.Com_Dict["%s" % port[0]] = "%s" % port[1]  # 将串口列表数据进行处理
                self.combobox_firmware_port.addItem(port[0])                    #将串口数据添加到控件cbb_serial_select中

        if len(self.Com_Dict) == 0:                                         #如果没有检测到端口，使用消息框打印提示信息，检测不到串口
            print('Cannot detect serial port!')                             #在数据接收区显示“检测不到串口！”
        else:
            print("\nThe following serial ports are found:")
            for port in self.Com_Dict:
                print(port)

    #打印串口波特率
    def port_select(self):
        if self.combobox_firmware_port.currentIndex() != -1:
            str_serial_port = "The current baud port is: " + self.combobox_firmware_port.currentText()
            print(str_serial_port)

    #串口选择打印提示信息
    def baud_select(self):
        str_baud = "The current baud baud is: " + self.combobox_firmware_baud.currentText()
        print(str_baud)

    #上传代码到机器狗
    def upload_bin(self):
        if self.combobox_firmware_port.currentText() != '':
            print("\nStart uploading the firmware to ESP32S3.")
            self.progressBar_firmware.setValue(0)
            if platform.system() == "Windows":
                upload_cmd = self.cmd.PYTHON + self.cmd.SPACE + self.cmd.ESPTOOL + self.cmd.SPACE + self.cmd.PORT + self.cmd.SPACE + self.combobox_firmware_port.currentText() + self.cmd.SPACE \
                             + self.cmd.BAUD + self.cmd.SPACE + self.combobox_firmware_baud.currentText() + self.cmd.SPACE \
                             + self.cmd.CHIP + self.cmd.SPACE + self.cmd.ESP32 + self.cmd.SPACE + self.cmd.CONFIG + self.cmd.SPACE \
                             + self.cmd.OTA_DATA_INITAL + self.cmd.SPACE \
                             + self.cmd.BOOTLOADER + self.cmd.SPACE \
                             + self.cmd.PARTITION_TABLE + self.cmd.SPACE \
                             + self.cmd.FIRMWARE + self.cmd.SPACE \
                             + self.cmd.MUSIC
            else:
                upload_cmd = self.cmd.PYTHON3 + self.cmd.SPACE + self.cmd.ESPTOOL + self.cmd.SPACE + self.cmd.PORT + self.cmd.SPACE + self.combobox_firmware_port.currentText() + self.cmd.SPACE \
                             + self.cmd.BAUD + self.cmd.SPACE + self.combobox_firmware_baud.currentText() + self.cmd.SPACE \
                             + self.cmd.CHIP + self.cmd.SPACE + self.cmd.ESP32 + self.cmd.SPACE + self.cmd.CONFIG + self.cmd.SPACE \
                             + self.cmd.OTA_DATA_INITAL + self.cmd.SPACE \
                             + self.cmd.BOOTLOADER + self.cmd.SPACE \
                             + self.cmd.PARTITION_TABLE + self.cmd.SPACE \
                             + self.cmd.FIRMWARE + self.cmd.SPACE \
                             + self.cmd.MUSIC
            print(upload_cmd)
            try:
                p = subprocess.Popen(upload_cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                num = 0
                while p.poll() != 0:
                    for line in iter(p.stdout.readline, b''):
                        s = line.split(b'\r\n')
                        print(s[0])
                        if line.find(b'A fatal error occurred') != -1:
                            print("Can't download it!")
                            break
                        elif line.find(b'Failed to execute script esptool') != -1:
                            print('Choose the wrong serial port!')
                        elif line.find(b'Hash of data verified.') != -1:
                            num += 1
                        elif line.find(b'Hard resetting via RTS pin...') != -1:
                            print('Upload Complete.')
                            break
                        else:
                            if line.find(b"Writing at") == -1:
                                pass
                            else:
                                if num >= 3:
                                    buf1 = line.split(b'(')
                                    buf2 = buf1[1].split(b' %')
                                    self.pyserial_chn.emit(int(buf2[0]))
                                else:
                                    pass
                p.stdout.close()  # 关闭管道连接
                p.kill()  # 结束线程
            except Exception as e:
                print(e)
            self.button_firmware_download.setEnabled(True)
            stop_thread(self.download_thread)
        else:
            print("Please scan the serial port first.")
            self.button_firmware_download.setEnabled(True)
            stop_thread(self.download_thread)
    #进度条数值设置
    def progressBar_show(self, data):
        self.progressBar_firmware.setValue(data)

    #创建一个下载线程
    def download(self):
        self.button_firmware_download.setEnabled(False)
        self.download_thread = threading.Thread(target=self.upload_bin)
        self.download_thread.start()

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    myshow = ESP32S3_Firmware()
    myshow.show()
    sys.exit(app.exec_())
