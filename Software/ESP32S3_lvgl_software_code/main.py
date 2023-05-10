#!/usr/bin/python3
# -*- coding:utf-8 -*-
# @Time: 2023/05/05
# @Author: 林镇涛
# @Company: Freenove

import os
import sys
import threading
import Thread
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from Client import Client
import Command
from ui.ui_main import Ui_form_main
from firmware import ESP32S3_Firmware
import io
import cv2
import numpy as np
import struct
from PIL import Image
import time

class ESP32S3_MainWindow(QtWidgets.QMainWindow,Ui_form_main):
    py_infomation_chn_1 = QtCore.pyqtSignal(str, str, str)
    py_infomation_chn_2 = QtCore.pyqtSignal(str, str, bytes)
    py_infomation_chn_3 = QtCore.pyqtSignal()

    def __init__(self):
        super(ESP32S3_MainWindow,self).__init__()
        self.setupUi(self)
        self.setFixedSize(940,380)                      #将界面设置为不可更改大小
        self.ip_address = None                          #用来存放IP地址
        self.client = Client()                          #用来操作WiFi对应的函数
        self.cmd=None                                   #用来操作socket指令传输
        self.video=None                                 #用来操作socket视频流传输
        self.command = Command.COMMAND()                #通讯命令解析
        self.receive_remote_file_state = False          #变量：大文件分多次接收时用来指示每次是否已经接收完单次数据
        self.send_local_file_state = False              #变量：大文件分多次发送时用来指示每次是否已经发送完单次数据
        self.receive_data_number = 0                    #变量：用来记录接收到的文件数据量
        self.send_data_number = 0                       #变量：用来记录发送过去的文件数据量
        self.read_data_state = False                    #标志：用来判断是否处于接收文件模式
        self.write_data_state = False                   #标志：用来判断是否处于发送文件模式
        self.camera_angle = 0                           #变量：用来记录视频的旋转角度
        self.video_stream_state = False                 #变量：用来记录视频流线程是否开启

        self.setWindowIcon(QIcon('./image/Freenove.ico'))            #添加窗口logo
        #self.label_main_video_show.setScaledContents(True)           #设置图片自动缩放
        pixmap_image = QtGui.QPixmap('./image/Development_Kit.png')  #加载视频窗口图片
        self.label_main_video_show.setPixmap(pixmap_image)           #在视频显示框中显示产品图片

        ipValidator = QRegExpValidator(QRegExp('^((2[0-4]\d|25[0-5]|\d?\d|1\d{2})\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)$'))
        self.lineEdit_main_ip_address.setValidator(ipValidator)      #限制IP地址输入框，只能输入IP地址格式

        self.local_folder_check()                                    #检查本地文件夹是否存在，存在，则不理会，不存在，则新建文件夹
        self.connect()                                               #信号与槽函数

    #本地文件夹存在与否检查
    def local_folder_check(self):
        if os.path.exists("./music") is False:
            os.mkdir("./music")
        if os.path.exists("./picture") is False:
            os.mkdir("./picture")
        if os.path.exists("./router") is False:
            os.mkdir("./router")
        self.local_ip_file_read()  # 启动软件，获取一次IP文件记录的地址
    #IP文件读取
    def local_ip_file_read(self):
        if os.path.exists("./router/IP.txt") is True:#判断IP.txt是否存在，如果存在，读取IP地址
            file = open("./router/IP.txt", "r", encoding="utf-8")
            self.ip_address = file.readline()
            file.close()
        else:#如果IP.txt文件不存在，直接赋空值
            self.ip_address=str(None)
        print("Read IP.txt, IP: " + self.ip_address)
        self.lineEdit_main_ip_address.setText(self.ip_address)
    #IP文件写入
    def local_ip_file_write(self):
        if os.path.exists("./router/IP.txt") is True:
            os.remove("./router/IP.txt")
        file = open("./router/IP.txt", "w", encoding="utf-8")
        self.ip_address = self.lineEdit_main_ip_address.text()
        file.write(self.ip_address)
        file.close()

    #信号与槽函数
    def connect(self):
        self.lineEdit_main_ip_address.textChanged.connect(self.local_ip_file_write)           #IP地址输入框内容变动信号与槽函数
        self.button_main_connect_remote_ip.clicked.connect(self.btn_connect_remote_ip)        #WiFi连接按键点击事件信号与槽函数
        self.button_main_firmware_download.clicked.connect(self.btn_firmware_download)        #固件下载按键点击事件信号与槽函数
        self.button_main_scan_remote_file.clicked.connect(self.btn_scan_remote_file)          #扫描远程文件按键点击事件信号与槽函数
        self.treeWidget_remote.clicked.connect(self.tree_widget_remote_clicked)               #远程文件树形窗口点击事件信号与槽函数
        self.button_main_delete_remote_file.clicked.connect(self.btn_delete_treewidget_remote)#远程文件删除按键点击事件信号与槽函数
        self.button_main_scan_local_file.clicked.connect(self.btn_scan_local_file)            #扫描本地文件按键点击事件信号与槽函数
        self.button_main_delete_local_file.clicked.connect(self.btn_delete_treewidget_local)  #本地文件删除按键点击事件信号与槽函数
        self.button_main_receive_remote_file.clicked.connect(self.btn_receive_remote_file)    #接收远程文件按键点击事件信号与槽函数
        self.button_main_send_local_file.clicked.connect(self.btn_send_local_file)            #发送本地文件按键点击事件信号与槽函数
        self.button_main_video_show.clicked.connect(self.btn_video_show)                      #视频传输按键点击事件信号与槽函数
        self.button_main_video_ccw_90_degree.clicked.connect(lambda: self.wifi_rotate_video(self.button_main_video_ccw_90_degree))
        self.button_main_video_cw_90_degree.clicked.connect(lambda: self.wifi_rotate_video(self.button_main_video_cw_90_degree))
        self.py_infomation_chn_1.connect(self.add_treewidget_remote)                          #消息通道1关联到主线程的远程树形列表刷新函数
        self.py_infomation_chn_2.connect(self.local_file_write)                               #消息通道2关联到主线程的本地文件写入函数
        self.py_infomation_chn_3.connect(self.btn_scan_local_file)                            #消息通道3关联到主线程的本地树形列表刷新函数

    #wifi连接按键回调函数
    def btn_connect_remote_ip(self):
       try:
           if self.button_main_connect_remote_ip.text()=='Connect':
               self.client.cmd_open_socket(self.ip_address, 5000)
               self.button_main_connect_remote_ip.setText("Disconnect")
               self.cmd = threading.Thread(target=self.recvmassage)
               self.cmd.start()
           else:
               if self.video_stream_state is True:
                   self.client.video_stop_socket()
                   try:
                       Thread.stop_thread(self.video)
                   except:
                       pass
                   pixmap_image = QtGui.QPixmap('./image/Development_Kit.png')  # 加载视频窗口图片
                   self.label_main_video_show.setPixmap(pixmap_image)  # 在视频显示框中显示产品图片
                   self.button_main_video_show.setText("Open video")
                   self.client.video_flag = False

               self.client.cmd_stop_socket()
               try:
                   Thread.stop_thread(self.cmd)
               except:
                   pass
               self.client.cmd_flag = False
               self.read_data_state = False
               self.write_data_state = False
               self.button_main_connect_remote_ip.setText("Connect")
       except Exception as e:
           print(e)

    #固件烧录按键回调函数
    def btn_firmware_download(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                self.firmwareWindow = ESP32S3_Firmware()
                self.firmwareWindow.setWindowModality(Qt.ApplicationModal)
                self.firmwareWindow.show()
        except Exception as e:
            print(e)

    #获取远程目录文件按键回调函数
    def btn_scan_remote_file(self):
        try:
            if self.button_main_connect_remote_ip.text()=="Disconnect":
                if self.read_data_state==False and self.write_data_state==False:
                    self.client.cmd_send_data(("L#/#all#0#0#\r\n").encode("utf-8"))
        except:
            pass

    #文件树删除远程文件/目录
    def delete_treewidget_remote(self):
        # 防止item是root时，root无父结点报错，要使用下面的写法
        rootFather = self.treeWidget_remote.invisibleRootItem()  # 获得根节点root的不可见的父节点
        for item in self.treeWidget_remote.selectedItems():
            if item.text(0)!="/":
                if item.text(1)=="folder":
                    while item.childCount()!=0:
                        (item.parent() or rootFather).removeChild(item.child(0))
                else:
                    (item.parent() or rootFather).removeChild(item)
            elif item.text(0)=="/" and item.text(1)=="folder":
                (item.parent() or rootFather).removeChild(item)

    #树形目录点击事件
    def tree_widget_remote_clicked(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                item = self.treeWidget_remote.currentItem()  # 获得当前单击项
                if  item.text(1)=="folder":
                    if item.childCount() != 0:
                        self.delete_treewidget_remote()
                    buf = "L#/" + item.text(0) + "#all#0#0#0#\r\n"
                    self.client.cmd_send_data(buf.encode("utf-8"))
        except:
            pass

    #远程删除按键回调函数
    def btn_delete_treewidget_remote(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                # 防止item是root时，root无父结点报错，要使用下面的写法
                rootFather = self.treeWidget_remote.invisibleRootItem()  # 获得根节点root的不可见的父节点
                for item in self.treeWidget_remote.selectedItems():
                    if item.text(0)!="/":
                        if item.text(1)=="folder":
                            while item.childCount()!=0:
                                buf = "D#/"+item.text(0)+"#"+item.child(0).text(0)+"#0#0#0#\r\n"
                                self.client.cmd_send_data(buf.encode("utf-8"))
                                (item.parent() or rootFather).removeChild(item.child(0))
                                time.sleep(0.3)
                            buf = "D#/" + item.text(0) + "#all#0#0#0#\r\n"
                            self.client.cmd_send_data(buf.encode("utf-8"))
                        else:
                            buf = "D#/"+item.parent().text(0)+"#"+item.text(0)+"#0#0#0#\r\n"
                            self.client.cmd_send_data(buf.encode("utf-8"))
                        (item.parent() or rootFather).removeChild(item)
        except:
            pass

    #文件树添加本地文件目录
    def btn_scan_local_file(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                for totol_dir in os.listdir("./"):
                    if totol_dir == "router":
                        item = self.treeWidget_local.topLevelItem(0).child(0)
                        while item.childCount() != 0:
                            item.parent().removeChild(item.child(0))
                        for dir in os.listdir(totol_dir):
                            item = self.treeWidget_local.topLevelItem(0).child(0)
                            child1 = QTreeWidgetItem(item)
                            child1.setText(0, str(dir))
                            child1.setText(1, str(os.path.getsize("./router/" + dir)))
                    elif totol_dir == "picture":
                        item = self.treeWidget_local.topLevelItem(0).child(1)
                        while item.childCount() != 0:
                            item.parent().removeChild(item.child(0))
                        for dir in os.listdir(totol_dir):
                            item = self.treeWidget_local.topLevelItem(0).child(1)
                            child1 = QTreeWidgetItem(item)
                            child1.setText(0, str(dir))
                            child1.setText(1, str(os.path.getsize("./picture/" + dir)))
                    elif totol_dir == "music":
                        item = self.treeWidget_local.topLevelItem(0).child(2)
                        while item.childCount() != 0:
                            item.parent().removeChild(item.child(0))
                        for dir in os.listdir(totol_dir):
                            item = self.treeWidget_local.topLevelItem(0).child(2)
                            child1 = QTreeWidgetItem(item)
                            child1.setText(0, str(dir))
                            child1.setText(1, str(os.path.getsize("./music/"+dir)))
                self.treeWidget_local.expandAll()
        except Exception as e:
            print(e)

    #本地删除按键回调函数
    def btn_delete_treewidget_local(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                item = self.treeWidget_local.currentItem()
                if item.text(0) != "/" and item.text(1) != "folder":
                    str = "./" + item.parent().text(0) + "/" + item.text(0)
                    os.remove(str)
                    item.parent().removeChild(item)
        except Exception as e:
            print(e)

    #接收远程文件按键回调函数
    def btn_receive_remote_file(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                if self.button_main_connect_remote_ip.text()=="Disconnect":
                    item = self.treeWidget_remote.currentItem()
                    if (item.text(0)!="/") and (item.text(1)!="folder"):
                        str = "R#/" + item.parent().text(0) + "#" + item.text(0) + "#" + item.text(1) + "#0#\r\n"
                        self.client.cmd_send_data(str.encode("utf-8"))
                    else:
                        print("This is a folder, please select a file. Then click the Receive button.")
        except:
            pass

    #发送本地文件按键回调函数
    def btn_send_local_file(self):
        try:
            if self.read_data_state == False and self.write_data_state == False:
                item = self.treeWidget_local.currentItem()
                if (item.text(0)!="/") and (item.text(1)!="folder"):
                    self.button_main_receive_remote_file.setCheckable(False)
                    self.button_main_send_local_file.setCheckable(False)
                    path = item.parent().text(0)
                    name = item.text(0)
                    size = item.text(1)
                    position = 0
                    buf = "./" + path + "/" + name
                    file = open(buf, "rb")
                    self.write_data_state = True
                    if (int(size)>=1024):
                        count  = int(size) // 1024
                        rest = int(size) % 1024
                        buf = "W#/" + path + "#" + name + "#" + size + "#" + str("1024") + "#"
                        print(buf)
                        self.client.cmd_send_data(buf.encode("utf-8"))
                        self.send_data_number = 0
                        for i in range(count):
                            self.send_data_number = i*1024
                            print(self.send_data_number, int(size))
                            file.seek(i*1024)
                            data = file.read(1024)
                            self.client.cmd_send_data(data)
                            time.sleep(0.002)
                        self.send_data_number = count*1024 + rest
                        print(self.send_data_number, int(size))
                        file.seek(count * 1024)
                        data = file.read(rest)
                        self.client.cmd_send_data(data)
                    else:
                        buf = "W#/" + path + "#" + name + "#" + size + "#" + str(position) + "#"
                        file.seek(0)
                        data = file.read(int(size))
                        self.client.cmd_send_data(buf.encode("utf-8"))
                        self.client.cmd_send_data(data)
                    self.write_data_state = False
                    file.close()
                    pass
                else:
                    print("This is a folder, please select a file. Then click the Send button.")
        except:
            pass

    #视频流开启和关闭函数
    def btn_video_show(self):
        if self.read_data_state == False and self.write_data_state == False:
            if self.client.cmd_flag is True: #确保已经连接到client_cmd服务器上。
                if self.button_main_video_show.text()=="Open video":     #如果还没打开视频流
                    self.video_stream_state = True
                    self.button_main_video_show.setText("Close video")
                    buf = "V#/#all#1#0#"
                    self.client.cmd_send_data(buf.encode("utf-8"))
                    time.sleep(1)
                    self.client.video_open_socket(self.ip_address, 8000)
                    self.video = threading.Thread(target=self.video_stream)
                    self.video.start()

                elif self.button_main_video_show.text()=="Close video":
                    self.video_stream_state = False
                    self.button_main_video_show.setText("Open video")
                    buf = "V#/#all#0#0#"
                    self.client.cmd_send_data(buf.encode("utf-8"))
                    self.client.video_stop_socket()
                    try:
                        Thread.stop_thread(self.video)
                    except Exception as e:
                        print(e)
                    pixmap_image = QtGui.QPixmap('./image/Development_Kit.png')  # 加载视频窗口图片
                    self.label_main_video_show.setPixmap(pixmap_image)  # 在视频显示框中显示产品图片

    #判断是否是jpg格式
    def socket_image_4_butes(self,buf):
        bValid = True
        if buf[6:10] in (b'JFIF', b'Exif'):
            if not buf.rstrip(b'\0\r\n').endswith(b'\xff\xd9'):
                bValid = False
        else:
            try:
                Image.open(io.BytesIO(buf)).verify()
            except:
                bValid = False
        return bValid
    #视频流数据解析
    def video_stream(self):
        try:
            self.connection = self.client.client_video_socket.makefile('rb')
        except Exception as e:
            self.button_main_video_show.setText('Open video')
            return None
        while self.client.video_flag is True:
            try:
                stream_bytes = self.connection.read(4)
                leng = struct.unpack('<L', stream_bytes[:4])
                jpg = self.connection.read(leng[0])
                if self.socket_image_4_butes(jpg):
                    buf_image = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
                    height, width, bytesPerComponent = buf_image.shape
                    cv2.cvtColor(buf_image, cv2.COLOR_BGR2RGB, buf_image)
                    QImg = QImage(buf_image.data, width, height, 3 * width, QImage.Format_RGB888)
                    transform = QTransform()
                    transform.scale(1, 1)
                    transform.rotate(self.camera_angle)
                    QImg = QImg.transformed(transform)
                    if self.button_main_video_show.text() == 'Close video':
                        self.label_main_video_show.setPixmap(QPixmap.fromImage(QImg))
            except Exception as e:
                print(e)
                break

    #视频显示-角度旋转函数
    def wifi_rotate_video(self, parameter):
        if parameter.text()=="CCW-90°":
            self.camera_angle -= 90
            if self.camera_angle <=-360:
                self.camera_angle = 0
        if parameter.text() == 'CW-90°':
            self.camera_angle += 90
            if self.camera_angle >= 360:
                self.camera_angle = 0

    #文件树添加远程文件目录函数
    def add_treewidget_remote(self, dir, name, size):
        if dir == "/":  # 添加文件名
            state = False
            item = self.treeWidget_remote.topLevelItem(0)#获取根目录并赋值给item
            for i in range(item.childCount()):#如果根目录下存在文件夹，判断新增文件名是否与旧文件名一致
                if item.child(i).text(0)==name:#如果一致，则无需新增文件夹
                    state=True
                    break
            if state==False:#如果新增文件夹名称之前文件夹名称并不一致，新增文件夹
                child1 = QTreeWidgetItem(item)
                child1.setText(0, name)
                child1.setText(1, str("folder"))
                self.treeWidget_remote.expandAll()
        elif (dir!="/")and(int(size)!=0):#添加文件名
            item = self.treeWidget_remote.currentItem()  # 获得当前单击项
            node = QTreeWidgetItem(item)
            node.setText(0, name)
            node.setText(1, size)
            self.treeWidget_remote.currentItem().setExpanded(1)

    #写入本地文件函数
    def local_file_write(self, path, name, content):
        if path=="/router" or path=="/picture" or path=="/music":
            file = None
            buf = "." + path
            if os.path.exists(buf) is False:
                print(buf + " is not exists. Create it.")
            buf = "." + path + "/" + name
            if os.path.exists(buf) is True:
                file = open(buf, "ab")
            else:
                file = open(buf, "wb")
            for i in content:
                translate = struct.pack("B", i)
                file.write(translate)
            file.close()
        else:
            print("Only router, picture, and music can be read.")

    #消息解析函数
    def wifi_message_parse(self, recv_data):
        if recv_data!=None:
            if self.read_data_state == True:
                try:
                    if recv_data!=b"OK\r\n":
                        self.receive_data_number += len(recv_data)
                        print(self.receive_data_number, int(self.read_data[3]))
                        try:
                            self.py_infomation_chn_2.emit(self.read_data[1].decode("utf-8"), self.read_data[2].decode("utf-8"), recv_data)
                        except Exception as e:
                            print(e)
                    else:
                        try:
                            self.py_infomation_chn_3.emit()
                        except Exception as e:
                            print(e)
                        self.read_data_state= False
                        self.receive_data_number=0
                except Exception as e:
                    print(e)
            else:
                try:
                    cmdArray = recv_data.split(b"\r\n")
                    for each_array in cmdArray:
                        parameter = each_array.split(self.command.INTERVAL_CHAR)#对#进行分隔
                        if parameter[0]==self.command.READ_MODE:#读取命令
                            if parameter[1]==b"/router" or parameter[1]==b"/picture" or parameter[1]==b"/music":
                                self.read_data = parameter
                                self.read_data_state=True
                                self.receive_data_number = 0
                        elif parameter[0]==self.command.LIST_MODE:#查询目录命令
                            self.py_infomation_chn_1.emit(parameter[1].decode("utf-8"),parameter[2].decode("utf-8"),parameter[3].decode("utf-8"))
                        elif parameter[0]==b"OK":
                            pass
                except Exception as e:
                    print(e)

    #接收esp32s3数据
    def recvmassage(self):
            try:
                while True:
                    recv_data = self.client.cmd_receive_data()
                    if recv_data!=None:
                        self.wifi_message_parse(recv_data)
                    else:
                        try:
                            self.button_main_connect_remote_ip.setText("Connect")
                            Thread.stop_thread(self.cmd)
                        except Exception as e:
                            print(e)
            except Exception as e:
                print(e)

if __name__ == '__main__':
    QtCore.QCoreApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling)
    app = QApplication(sys.argv)
    myshow = ESP32S3_MainWindow()
    myshow.show()
    sys.exit(app.exec_())

