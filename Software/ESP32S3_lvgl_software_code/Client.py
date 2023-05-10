#!/usr/bin/python
# -*- coding: utf-8 -*-
import socket

class Client:
    def __init__(self):
        self.client_cmd_socket=None    #命令端口，常见值为5000
        self.client_video_socket=None  #视频端口，常见值为8000
        self.client_cmd_ip=None        #命令socket IP
        self.client_cmd_port=None      #命令socket Port
        self.client_video_ip=None      #视频socket IP
        self.client_video_port=None    #视频socket Port

        self.cmd_flag = False
        self.video_flag = False

    #将Socket关联到对应的IP地址和端口
    def cmd_open_socket(self, ip, port):
        try:
            self.client_cmd_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_cmd_ip = ip
            self.client_cmd_port = port
            self.client_cmd_socket.connect((self.client_cmd_ip, self.client_cmd_port))
            print("Open socket_cmd_connect Successful.")
            self.cmd_flag=True
        except Exception as e:
            print("Connect to server Faild!: Server IP is right? Server is opend? " + ip)
            self.cmd_flag = False
    def video_open_socket(self, ip, port):
        try:
            self.client_video_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_video_ip = ip
            self.client_video_port = port
            self.client_video_socket.connect((self.client_video_ip, self.client_video_port))
            print("Open socket_video_connect Successful !")
            self.video_flag = True
        except Exception as e:
            print("Connect to server Faild!: Server IP is right? Server is opend? " + ip)
            self.video_flag = False

    #关闭socket对象
    def cmd_stop_socket(self):
        try:
            self.client_cmd_socket.shutdown(2)
            self.client_cmd_socket.close()
            print("Stop client_cmd_socket success.\r\n")
        except:
            print("Stop client_cmd_socket failed.\r\n")

    def video_stop_socket(self):
        try:
            self.client_video_socket.shutdown(2)
            self.client_video_socket.close()
            print("Stop client_video_socket success.")
        except:
            print("Stop client_video_socket failed.")

    #判断socket是否仍然连接着
    def cmd_socket_isconnect(self):
        return self.cmd_flag
    def video_socket_isconnect(self):
        return self.video_flag

    #发送函数
    def cmd_send_data(self,s):
        try:
            if self.cmd_socket_isconnect()==True:
                self.client_cmd_socket.send(s)
        except:
            print("cmd send data failed.")
    def video_send_data(self,s):
        try:
            if self.video_socket_isconnect() is True:
                self.client_video_socket.send(s.encode('utf-8'))
        except:
            print("video send data failed.")

    #接收函数
    def cmd_receive_data(self):
        data = None
        if self.cmd_flag==True:
            try:
                data = self.client_cmd_socket.recv(1024)
            except Exception as e:
                print(e)
        return data

    def video_receive_data(self):
        data = ""
        try:
            self.video_open_socket(self.client_video_ip, self.client_video_port)
            data = self.client_video_socket.recv(2048).decode('utf-8')
        except:
            print("video receive data failed.")
        return data

    def cmd_read_accept(self):
        return self.client_cmd_socket.recv(1024)


if __name__ == '__main__':
    pass

