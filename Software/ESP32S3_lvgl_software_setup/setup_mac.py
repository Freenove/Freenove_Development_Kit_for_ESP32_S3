import os
import sys
import time

flag=0x00
for x in range(1,4):
    if os.system("python3 -m pip install --upgrade pip --user") == 0:
        flag=flag | 0x01
        break
for x in range(1,4):
    if os.system("pip3 install pyserial==3.5") == 0:
        flag=flag | 0x02
        break
for x in range(1,4):
    if os.system("pip3 install PyQt5==5.15.4") == 0:
        flag=flag | 0x04
        break
for x in range(1,4):
    if os.system("pip3 install Pillow==9.1.1") == 0:
        flag=flag | 0x08
        break
for x in range(1,4):
    if os.system("pip3 install opencv-python-headless==4.5.4.60") == 0:
        flag=flag | 0x10
        break

if flag==0x1F:
        print("\nNow the installation is successful.")
else:
        print ("\nSome libraries have not been installed yet. Please run 'sudo python setup.py' again")

