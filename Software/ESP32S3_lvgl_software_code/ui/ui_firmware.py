# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_firmware.ui'
#
# Created by: PyQt5 UI code generator 5.15.4
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_widget_firmware(object):
    def setupUi(self, widget_firmware):
        widget_firmware.setObjectName("widget_firmware")
        widget_firmware.resize(610, 180)
        font = QtGui.QFont()
        font.setFamily("Arial")
        widget_firmware.setFont(font)
        self.label_firmware_port = QtWidgets.QLabel(widget_firmware)
        self.label_firmware_port.setGeometry(QtCore.QRect(30, 80, 101, 31))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_firmware_port.setFont(font)
        self.label_firmware_port.setContextMenuPolicy(QtCore.Qt.DefaultContextMenu)
        self.label_firmware_port.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.label_firmware_port.setFrameShape(QtWidgets.QFrame.Panel)
        self.label_firmware_port.setAlignment(QtCore.Qt.AlignCenter)
        self.label_firmware_port.setObjectName("label_firmware_port")
        self.combobox_firmware_port = QtWidgets.QComboBox(widget_firmware)
        self.combobox_firmware_port.setGeometry(QtCore.QRect(130, 80, 151, 31))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.combobox_firmware_port.setFont(font)
        self.combobox_firmware_port.setFocusPolicy(QtCore.Qt.NoFocus)
        self.combobox_firmware_port.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.combobox_firmware_port.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);")
        self.combobox_firmware_port.setMaxVisibleItems(18)
        self.combobox_firmware_port.setObjectName("combobox_firmware_port")
        self.button_firmware_download = QtWidgets.QPushButton(widget_firmware)
        self.button_firmware_download.setGeometry(QtCore.QRect(330, 20, 251, 31))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.button_firmware_download.setFont(font)
        self.button_firmware_download.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_firmware_download.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);")
        self.button_firmware_download.setObjectName("button_firmware_download")
        self.combobox_firmware_baud = QtWidgets.QComboBox(widget_firmware)
        self.combobox_firmware_baud.setGeometry(QtCore.QRect(430, 80, 151, 31))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.combobox_firmware_baud.setFont(font)
        self.combobox_firmware_baud.setFocusPolicy(QtCore.Qt.NoFocus)
        self.combobox_firmware_baud.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.combobox_firmware_baud.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);")
        self.combobox_firmware_baud.setMaxVisibleItems(18)
        self.combobox_firmware_baud.setInsertPolicy(QtWidgets.QComboBox.InsertAtBottom)
        self.combobox_firmware_baud.setSizeAdjustPolicy(QtWidgets.QComboBox.AdjustToContentsOnFirstShow)
        self.combobox_firmware_baud.setObjectName("combobox_firmware_baud")
        self.combobox_firmware_baud.addItem("")
        self.combobox_firmware_baud.addItem("")
        self.combobox_firmware_baud.addItem("")
        self.combobox_firmware_baud.addItem("")
        self.combobox_firmware_baud.addItem("")
        self.combobox_firmware_baud.addItem("")
        self.label_firmware_baudrate = QtWidgets.QLabel(widget_firmware)
        self.label_firmware_baudrate.setGeometry(QtCore.QRect(330, 80, 101, 31))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_firmware_baudrate.setFont(font)
        self.label_firmware_baudrate.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);")
        self.label_firmware_baudrate.setFrameShape(QtWidgets.QFrame.Panel)
        self.label_firmware_baudrate.setAlignment(QtCore.Qt.AlignCenter)
        self.label_firmware_baudrate.setObjectName("label_firmware_baudrate")
        self.button_firmware_scan = QtWidgets.QPushButton(widget_firmware)
        self.button_firmware_scan.setGeometry(QtCore.QRect(30, 20, 251, 31))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.button_firmware_scan.setFont(font)
        self.button_firmware_scan.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_firmware_scan.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);")
        self.button_firmware_scan.setObjectName("button_firmware_scan")
        self.label_firmware_1 = QtWidgets.QLabel(widget_firmware)
        self.label_firmware_1.setGeometry(QtCore.QRect(30, 50, 251, 21))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_firmware_1.setFont(font)
        self.label_firmware_1.setStyleSheet("font: 10pt \"Arial\";")
        self.label_firmware_1.setObjectName("label_firmware_1")
        self.label_firmware_2 = QtWidgets.QLabel(widget_firmware)
        self.label_firmware_2.setGeometry(QtCore.QRect(30, 110, 251, 21))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_firmware_2.setFont(font)
        self.label_firmware_2.setStyleSheet("font: 10pt \"Arial\";")
        self.label_firmware_2.setObjectName("label_firmware_2")
        self.label_firmware_3 = QtWidgets.QLabel(widget_firmware)
        self.label_firmware_3.setGeometry(QtCore.QRect(330, 50, 251, 21))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_firmware_3.setFont(font)
        self.label_firmware_3.setStyleSheet("font: 10pt \"Arial\";")
        self.label_firmware_3.setObjectName("label_firmware_3")
        self.label_firmware_4 = QtWidgets.QLabel(widget_firmware)
        self.label_firmware_4.setGeometry(QtCore.QRect(330, 110, 251, 21))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_firmware_4.setFont(font)
        self.label_firmware_4.setStyleSheet("font: 10pt \"Arial\";")
        self.label_firmware_4.setObjectName("label_firmware_4")
        self.progressBar_firmware = QtWidgets.QProgressBar(widget_firmware)
        self.progressBar_firmware.setGeometry(QtCore.QRect(30, 140, 551, 23))
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.progressBar_firmware.setFont(font)
        self.progressBar_firmware.setStyleSheet("font: 10pt \"Arial\";")
        self.progressBar_firmware.setProperty("value", 0)
        self.progressBar_firmware.setObjectName("progressBar_firmware")

        self.retranslateUi(widget_firmware)
        QtCore.QMetaObject.connectSlotsByName(widget_firmware)

    def retranslateUi(self, widget_firmware):
        _translate = QtCore.QCoreApplication.translate
        widget_firmware.setWindowTitle(_translate("widget_firmware", "Freenove"))
        self.label_firmware_port.setText(_translate("widget_firmware", "Port"))
        self.button_firmware_download.setText(_translate("widget_firmware", "Download"))
        self.combobox_firmware_baud.setItemText(0, _translate("widget_firmware", "2000000"))
        self.combobox_firmware_baud.setItemText(1, _translate("widget_firmware", "115200"))
        self.combobox_firmware_baud.setItemText(2, _translate("widget_firmware", "230400"))
        self.combobox_firmware_baud.setItemText(3, _translate("widget_firmware", "460800"))
        self.combobox_firmware_baud.setItemText(4, _translate("widget_firmware", "921600"))
        self.combobox_firmware_baud.setItemText(5, _translate("widget_firmware", "2000000"))
        self.label_firmware_baudrate.setText(_translate("widget_firmware", "Baud"))
        self.button_firmware_scan.setText(_translate("widget_firmware", "Scan Serial"))
        self.label_firmware_1.setText(_translate("widget_firmware", "1, Please connect ESP32 to your computer."))
        self.label_firmware_2.setText(_translate("widget_firmware", "2, Please select the correct serial port."))
        self.label_firmware_3.setText(_translate("widget_firmware", "3, Please click on it to upload the code."))
        self.label_firmware_4.setText(_translate("widget_firmware", "4, Baud rate Controls the upload speed."))
