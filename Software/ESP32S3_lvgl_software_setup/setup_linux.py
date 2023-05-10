import os
import sys
import time

flag=0x00
for x in range(1,4):
	if os.system("sudo apt-get update") == 0:
		flag=flag | 0x01
		break
for x in range(1,4):
	if os.system("sudo apt install python3-pip") == 0:
		flag=flag | 0x02
		break
for x in range(1,4):
	if os.system("sudo pip3 install pyserial") == 0:
		flag=flag | 0x04
		break
for x in range(1,4):
	if os.system("sudo pip3 install PyQt5") == 0:
		flag=flag | 0x08
		break
for x in range(1,4):
	if os.system("sudo apt install pyqt5*") == 0:
		flag=flag | 0x10
		break
for x in range(1,4):
	if os.system("sudo apt install libopencv-dev python3-opencv") == 0:
		flag=flag | 0x20
		break

if flag==0x3F:
        print("\nNow the installation is successful.")
else:
        print ("\nSome libraries have not been installed yet. Please run 'sudo python setup.py' again")

