# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_main.ui'
#
# Created by: PyQt5 UI code generator 5.15.4
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_form_main(object):
    def setupUi(self, form_main):
        form_main.setObjectName("form_main")
        form_main.resize(940, 380)
        font = QtGui.QFont()
        font.setFamily("Arial")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        form_main.setFont(font)
        form_main.setAutoFillBackground(True)
        form_main.setStyleSheet("font: 10pt \"Arial\";")
        self.label_main_video_show = QtWidgets.QLabel(form_main)
        self.label_main_video_show.setGeometry(QtCore.QRect(10, 50, 320, 240))
        self.label_main_video_show.setStyleSheet("font: 10pt \"Arial\";")
        self.label_main_video_show.setFrameShape(QtWidgets.QFrame.Box)
        self.label_main_video_show.setText("")
        self.label_main_video_show.setAlignment(QtCore.Qt.AlignCenter)
        self.label_main_video_show.setObjectName("label_main_video_show")
        self.button_main_video_ccw_90_degree = QtWidgets.QPushButton(form_main)
        self.button_main_video_ccw_90_degree.setGeometry(QtCore.QRect(10, 300, 101, 31))
        self.button_main_video_ccw_90_degree.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_video_ccw_90_degree.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);")
        self.button_main_video_ccw_90_degree.setObjectName("button_main_video_ccw_90_degree")
        self.button_main_video_cw_90_degree = QtWidgets.QPushButton(form_main)
        self.button_main_video_cw_90_degree.setGeometry(QtCore.QRect(120, 300, 101, 31))
        self.button_main_video_cw_90_degree.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_video_cw_90_degree.setStyleSheet("background-color: rgb(216, 216, 216);\n"
"font: 10pt \"Arial\";")
        self.button_main_video_cw_90_degree.setObjectName("button_main_video_cw_90_degree")
        self.button_main_firmware_download = QtWidgets.QPushButton(form_main)
        self.button_main_firmware_download.setGeometry(QtCore.QRect(230, 340, 101, 31))
        self.button_main_firmware_download.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_firmware_download.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_firmware_download.setObjectName("button_main_firmware_download")
        self.lineEdit_main_ip_address = QtWidgets.QLineEdit(form_main)
        self.lineEdit_main_ip_address.setGeometry(QtCore.QRect(10, 340, 101, 31))
        self.lineEdit_main_ip_address.setStyleSheet("font: 10pt \"Arial\";")
        self.lineEdit_main_ip_address.setAlignment(QtCore.Qt.AlignCenter)
        self.lineEdit_main_ip_address.setObjectName("lineEdit_main_ip_address")
        self.button_main_connect_remote_ip = QtWidgets.QPushButton(form_main)
        self.button_main_connect_remote_ip.setGeometry(QtCore.QRect(120, 340, 101, 31))
        self.button_main_connect_remote_ip.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_connect_remote_ip.setStyleSheet("background-color: rgb(216, 216, 216);\n"
"font: 10pt \"Arial\";")
        self.button_main_connect_remote_ip.setObjectName("button_main_connect_remote_ip")
        self.button_main_video_show = QtWidgets.QPushButton(form_main)
        self.button_main_video_show.setGeometry(QtCore.QRect(230, 300, 101, 31))
        self.button_main_video_show.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_video_show.setStyleSheet("background-color: rgb(216, 216, 216);\n"
"font: 10pt \"Arial\";")
        self.button_main_video_show.setObjectName("button_main_video_show")
        self.button_main_receive_remote_file = QtWidgets.QPushButton(form_main)
        self.button_main_receive_remote_file.setGeometry(QtCore.QRect(340, 300, 291, 31))
        self.button_main_receive_remote_file.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_receive_remote_file.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_receive_remote_file.setObjectName("button_main_receive_remote_file")
        self.button_main_delete_remote_file = QtWidgets.QPushButton(form_main)
        self.button_main_delete_remote_file.setGeometry(QtCore.QRect(340, 340, 291, 31))
        self.button_main_delete_remote_file.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_delete_remote_file.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_delete_remote_file.setObjectName("button_main_delete_remote_file")
        self.button_main_send_local_file = QtWidgets.QPushButton(form_main)
        self.button_main_send_local_file.setGeometry(QtCore.QRect(640, 300, 291, 31))
        self.button_main_send_local_file.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_send_local_file.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_send_local_file.setObjectName("button_main_send_local_file")
        self.button_main_delete_local_file = QtWidgets.QPushButton(form_main)
        self.button_main_delete_local_file.setGeometry(QtCore.QRect(640, 340, 291, 31))
        self.button_main_delete_local_file.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_delete_local_file.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_delete_local_file.setObjectName("button_main_delete_local_file")
        self.button_main_scan_remote_file = QtWidgets.QPushButton(form_main)
        self.button_main_scan_remote_file.setGeometry(QtCore.QRect(340, 10, 291, 31))
        self.button_main_scan_remote_file.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_scan_remote_file.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_scan_remote_file.setObjectName("button_main_scan_remote_file")
        self.button_main_scan_local_file = QtWidgets.QPushButton(form_main)
        self.button_main_scan_local_file.setGeometry(QtCore.QRect(640, 10, 291, 31))
        self.button_main_scan_local_file.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_scan_local_file.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_scan_local_file.setObjectName("button_main_scan_local_file")
        self.treeWidget_remote = QtWidgets.QTreeWidget(form_main)
        self.treeWidget_remote.setGeometry(QtCore.QRect(340, 50, 291, 241))
        self.treeWidget_remote.setAcceptDrops(False)
        self.treeWidget_remote.setFrameShape(QtWidgets.QFrame.Box)
        self.treeWidget_remote.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.treeWidget_remote.setMidLineWidth(1)
        self.treeWidget_remote.setSizeAdjustPolicy(QtWidgets.QAbstractScrollArea.AdjustToContents)
        self.treeWidget_remote.setProperty("showDropIndicator", False)
        self.treeWidget_remote.setDragDropMode(QtWidgets.QAbstractItemView.NoDragDrop)
        self.treeWidget_remote.setAlternatingRowColors(False)
        self.treeWidget_remote.setAutoExpandDelay(-1)
        self.treeWidget_remote.setColumnCount(2)
        self.treeWidget_remote.setObjectName("treeWidget_remote")
        item_0 = QtWidgets.QTreeWidgetItem(self.treeWidget_remote)
        self.treeWidget_remote.header().setDefaultSectionSize(150)
        self.treeWidget_remote.header().setMinimumSectionSize(40)
        self.treeWidget_local = QtWidgets.QTreeWidget(form_main)
        self.treeWidget_local.setGeometry(QtCore.QRect(640, 50, 291, 241))
        self.treeWidget_local.setAcceptDrops(False)
        self.treeWidget_local.setFrameShape(QtWidgets.QFrame.Box)
        self.treeWidget_local.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.treeWidget_local.setMidLineWidth(1)
        self.treeWidget_local.setSizeAdjustPolicy(QtWidgets.QAbstractScrollArea.AdjustToContents)
        self.treeWidget_local.setProperty("showDropIndicator", False)
        self.treeWidget_local.setDragDropMode(QtWidgets.QAbstractItemView.NoDragDrop)
        self.treeWidget_local.setAlternatingRowColors(False)
        self.treeWidget_local.setAutoExpandDelay(-1)
        self.treeWidget_local.setColumnCount(2)
        self.treeWidget_local.setObjectName("treeWidget_local")
        item_0 = QtWidgets.QTreeWidgetItem(self.treeWidget_local)
        item_1 = QtWidgets.QTreeWidgetItem(item_0)
        item_1 = QtWidgets.QTreeWidgetItem(item_0)
        item_1 = QtWidgets.QTreeWidgetItem(item_0)
        self.treeWidget_local.header().setDefaultSectionSize(150)
        self.treeWidget_local.header().setMinimumSectionSize(40)
        self.button_main_Freenove = QtWidgets.QPushButton(form_main)
        self.button_main_Freenove.setGeometry(QtCore.QRect(10, 10, 321, 31))
        self.button_main_Freenove.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button_main_Freenove.setStyleSheet("font: 10pt \"Arial\";\n"
"background-color: rgb(216, 216, 216);\n"
"")
        self.button_main_Freenove.setObjectName("button_main_Freenove")

        self.retranslateUi(form_main)
        QtCore.QMetaObject.connectSlotsByName(form_main)

    def retranslateUi(self, form_main):
        _translate = QtCore.QCoreApplication.translate
        form_main.setWindowTitle(_translate("form_main", "Freenove"))
        self.button_main_video_ccw_90_degree.setText(_translate("form_main", "CCW-90°"))
        self.button_main_video_cw_90_degree.setText(_translate("form_main", "CW-90°"))
        self.button_main_firmware_download.setText(_translate("form_main", "Firmware"))
        self.lineEdit_main_ip_address.setText(_translate("form_main", "IP Address"))
        self.button_main_connect_remote_ip.setText(_translate("form_main", "Connect"))
        self.button_main_video_show.setText(_translate("form_main", "Open video"))
        self.button_main_receive_remote_file.setText(_translate("form_main", "Receive remote file"))
        self.button_main_delete_remote_file.setText(_translate("form_main", "Delete remote file"))
        self.button_main_send_local_file.setText(_translate("form_main", "Send local file"))
        self.button_main_delete_local_file.setText(_translate("form_main", "Delete local file"))
        self.button_main_scan_remote_file.setText(_translate("form_main", "Scan remote folder"))
        self.button_main_scan_local_file.setText(_translate("form_main", "Scan local folder"))
        self.treeWidget_remote.setSortingEnabled(True)
        self.treeWidget_remote.headerItem().setText(0, _translate("form_main", "Name"))
        self.treeWidget_remote.headerItem().setText(1, _translate("form_main", "Size"))
        __sortingEnabled = self.treeWidget_remote.isSortingEnabled()
        self.treeWidget_remote.setSortingEnabled(False)
        self.treeWidget_remote.topLevelItem(0).setText(0, _translate("form_main", "/"))
        self.treeWidget_remote.setSortingEnabled(__sortingEnabled)
        self.treeWidget_local.setSortingEnabled(True)
        self.treeWidget_local.headerItem().setText(0, _translate("form_main", "Name"))
        self.treeWidget_local.headerItem().setText(1, _translate("form_main", "Size"))
        __sortingEnabled = self.treeWidget_local.isSortingEnabled()
        self.treeWidget_local.setSortingEnabled(False)
        self.treeWidget_local.topLevelItem(0).setText(0, _translate("form_main", "/"))
        self.treeWidget_local.topLevelItem(0).child(0).setText(0, _translate("form_main", "router"))
        self.treeWidget_local.topLevelItem(0).child(0).setText(1, _translate("form_main", "folder"))
        self.treeWidget_local.topLevelItem(0).child(1).setText(0, _translate("form_main", "picture"))
        self.treeWidget_local.topLevelItem(0).child(1).setText(1, _translate("form_main", "folder"))
        self.treeWidget_local.topLevelItem(0).child(2).setText(0, _translate("form_main", "music"))
        self.treeWidget_local.topLevelItem(0).child(2).setText(1, _translate("form_main", "folder"))
        self.treeWidget_local.setSortingEnabled(__sortingEnabled)
        self.button_main_Freenove.setText(_translate("form_main", "FREENOVE"))
