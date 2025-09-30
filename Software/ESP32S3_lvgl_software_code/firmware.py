#!/usr/bin/python3
# -*- coding:utf-8 -*-
# @Time: 2025/09/30
# @Author: 林镇涛
# @Company: Freenove

import os
import sys                                 # Add system parameter object
import serial                              # Add system serial configuration object
import serial.tools.list_ports             # Add serial tools list object
from PyQt5 import QtWidgets,QtCore         # Add window object
from PyQt5.QtGui import QIcon              # Add icon object
import platform                            # Identify system platform
from Thread import *
import subprocess
from PyQt5.QtWidgets import QDialog, QVBoxLayout, QPushButton, QLabel

from ui.ui_firmware import Ui_widget_firmware # Import serial interface object
from Command import COMMAND                # Import esptool command parameters

# Add new firmware selection dialog class in ESP32S3_Firmware class
class FirmwareSelectionDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Select Firmware")
        self.setFixedSize(300, 150)
        self.selected_firmware = None
        
        layout = QVBoxLayout()
        
        label = QLabel("Please select firmware to upload:")
        layout.addWidget(label)
        
        btn_cfg1 = QPushButton("firmware_cfg1.bin")
        btn_cfg1.clicked.connect(lambda: self.select_firmware("firmware_cfg1.bin"))
        layout.addWidget(btn_cfg1)
        
        btn_cfg2 = QPushButton("firmware_cfg2.bin")
        btn_cfg2.clicked.connect(lambda: self.select_firmware("firmware_cfg2.bin"))
        layout.addWidget(btn_cfg2)
        
        self.setLayout(layout)
    
    def select_firmware(self, firmware):
        self.selected_firmware = firmware
        self.accept()

class ESP32S3_Firmware(QtWidgets.QWidget, Ui_widget_firmware):
    pyserial_chn = QtCore.pyqtSignal(int)
    enable_download_button = QtCore.pyqtSignal(bool)
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Add interface logo
        self.setWindowIcon(QIcon('./image/Freenove.ico'))                     # Add window logo

        # Variable declaration
        self.pyserial=serial.Serial()                                         # Serial object
        self.Com_Dict = {}                                                    # Detect all existing serial ports, store information in dictionary
        self.cmd = COMMAND()                                                  # Command object

        # Set signal and slot functions
        self.connect()

    def print_information(self):
        print('The operating system is: ' + platform.system())
        print("1. Please connect ESP32 to your computer.")
        print("2. Click Scan Serial to find the Serial port.")
        print("3. Select the serial port corresponding to ESP32.")
        print("4. Select baud rate, the download speed is determined by baud rate, too high baud rate may fail.")
        print("5. Click the download button and wait for the download to complete.")
    
    # Signal and slot section
    def connect(self):
        self.button_firmware_scan.clicked.connect(self.port_check)                   # Detect serial port and update to serial port dropdown box
        self.combobox_firmware_port.currentTextChanged.connect(self.port_select)     # Detect serial port changes
        self.combobox_firmware_baud.currentTextChanged.connect(self.baud_select)     # Detect baud rate changes
        self.button_firmware_download.clicked.connect(self.download)                 # Call esptool to upload firmware
        self.pyserial_chn.connect(self.progressBar_show)                             # Show download progress
        self.enable_download_button.connect(self.set_download_button_enabled)

    def set_download_button_enabled(self, enabled):
        self.button_firmware_download.setEnabled(enabled)

    # Serial port detection
    def port_check(self):
        port_list = list(serial.tools.list_ports.comports())                # Get serial port numbers and force convert to list and store in port_list
        self.combobox_firmware_port.clear()                                 # Clear serial port list data
        self.Com_Dict.clear()
        for port in port_list:                                              # Parse serial port list data, store in dictionary Com_Dict, and add to screen serial port dropdown box
            if port[0] != 'COM1':
                self.Com_Dict["%s" % port[0]] = "%s" % port[1]  # Process serial port list data
                self.combobox_firmware_port.addItem(port[0])                    # Add serial port data to control cbb_serial_select

        if len(self.Com_Dict) == 0:                                         # If no port is detected, use message box to print prompt information, cannot detect serial port
            print('Cannot detect serial port!')                             # Display "Cannot detect serial port!" in data receiving area
        else:
            print("\nThe following serial ports are found:")
            for port in self.Com_Dict:
                print(port)

    # Print serial port baud rate
    def port_select(self):
        if self.combobox_firmware_port.currentIndex() != -1:
            str_serial_port = "The current baud port is: " + self.combobox_firmware_port.currentText()
            print(str_serial_port)

    # Serial port selection print prompt information
    def baud_select(self):
        str_baud = "The current baud baud is: " + self.combobox_firmware_baud.currentText()
        print(str_baud)

    # Upload code to robot dog
    def upload_bin(self):
        if self.combobox_firmware_port.currentText() != '':
            print("\nStart uploading firmware to ESP32S3.")
            # Cannot directly operate UI in thread, use signal to update progress bar
            self.pyserial_chn.emit(0)
            
            # Determine firmware file based on user selection
            firmware_file = self.cmd.FIRMWARE_CFG1  # Default value
            if hasattr(self, 'selected_firmware'):
                if self.selected_firmware == "firmware_cfg2.bin":
                    # Need to define FIRMWARE_CFG2 constant in Command.py
                    firmware_file = self.cmd.FIRMWARE_CFG2 if hasattr(self.cmd, 'FIRMWARE_CFG2') else self.cmd.FIRMWARE_CFG1
            
            if platform.system() == "Windows":
                upload_cmd = self.cmd.PYTHON + self.cmd.SPACE + self.cmd.ESPTOOL + self.cmd.SPACE + self.cmd.PORT + self.cmd.SPACE + self.combobox_firmware_port.currentText() + self.cmd.SPACE \
                            + self.cmd.BAUD + self.cmd.SPACE + self.combobox_firmware_baud.currentText() + self.cmd.SPACE \
                            + self.cmd.CHIP + self.cmd.SPACE + self.cmd.ESP32 + self.cmd.SPACE + self.cmd.CONFIG + self.cmd.SPACE \
                            + self.cmd.OTA_DATA_INITAL + self.cmd.SPACE \
                            + self.cmd.BOOTLOADER + self.cmd.SPACE \
                            + self.cmd.PARTITION_TABLE + self.cmd.SPACE \
                            + firmware_file + self.cmd.SPACE \
                            + self.cmd.MUSIC
            else:
                upload_cmd = self.cmd.PYTHON3 + self.cmd.SPACE + self.cmd.ESPTOOL + self.cmd.SPACE + self.cmd.PORT + self.cmd.SPACE + self.combobox_firmware_port.currentText() + self.cmd.SPACE \
                            + self.cmd.BAUD + self.cmd.SPACE + self.combobox_firmware_baud.currentText() + self.cmd.SPACE \
                            + self.cmd.CHIP + self.cmd.SPACE + self.cmd.ESP32 + self.cmd.SPACE + self.cmd.CONFIG + self.cmd.SPACE \
                            + self.cmd.OTA_DATA_INITAL + self.cmd.SPACE \
                            + self.cmd.BOOTLOADER + self.cmd.SPACE \
                            + self.cmd.PARTITION_TABLE + self.cmd.SPACE \
                            + firmware_file + self.cmd.SPACE \
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
                            print("Cannot download!")
                            break
                        elif line.find(b'Failed to execute script esptool') != -1:
                            print('Wrong serial port selected!')
                        elif line.find(b'Hash of data verified.') != -1:
                            num += 1
                        elif line.find(b'Hard resetting via RTS pin...') != -1:
                            print('Upload complete.')
                            break
                        else:
                            if line.find(b"Writing at") == -1:
                                pass
                            else:
                                if num >= 3:
                                    buf1 = line.split(b'(')
                                    buf2 = buf1[1].split(b' %')
                                    # Use signal to update progress bar instead of direct call
                                    self.pyserial_chn.emit(int(buf2[0]))
                                else:
                                    pass
                p.stdout.close()
                p.kill()
            except Exception as e:
                print(e)
            # Use signal instead of directly operating button
            self.enable_download_button.emit(True)
            stop_thread(self.download_thread)
        else:
            print("Please scan the serial port first.")
            # Use signal instead of directly operating button
            self.enable_download_button.emit(True)
            stop_thread(self.download_thread)

    # Progress bar value setting
    def progressBar_show(self, data):
        self.progressBar_firmware.setValue(data)

    # Modify download method
    def download(self):
        # Create and show firmware selection dialog
        dialog = FirmwareSelectionDialog(self)
        if dialog.exec_() == QDialog.Accepted:
            # Save user selected firmware
            self.selected_firmware = dialog.selected_firmware
            print(f"Selected firmware: {self.selected_firmware}")
            
            # Start download process
            self.button_firmware_download.setEnabled(False)
            self.download_thread = threading.Thread(target=self.upload_bin)
            self.download_thread.start()
        else:
            print("Firmware selection cancelled")
            self.button_firmware_download.setEnabled(True)

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    myshow = ESP32S3_Firmware()
    myshow.show()
    sys.exit(app.exec_())