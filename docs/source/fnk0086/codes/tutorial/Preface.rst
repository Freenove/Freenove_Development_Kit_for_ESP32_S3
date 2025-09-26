.. _Preface:

##############################################################################
Preface
##############################################################################

ESP32-S3 WROOM
*****************************

ESP32-S3-WROOM-1 has launched a total of two antenna packages, PCB on-board antenna and IPEX antenna respectively. The PCB on-board antenna is an integrated antenna in the chip module itself, so it is convenient to carry and design. The IPEX antenna is a metal antenna derived from the integrated antenna of the chip module itself, which is used to enhance the signal of the module.

.. list-table:: 
   :width: 80%
   :header-rows: 1 
   :align: center
   :class: table-line
   
   * -  PCB on-board antenna
     -  IPEX antenna

   * -  |Preface00|
     -  |Preface01|

.. |Preface00| image:: ../_static/imgs/Preface/Preface00.png
.. |Preface01| image:: ../_static/imgs/Preface/Preface01.png

In this tutorial, the ESP32-S3 WROOM is designed based on the PCB on-board antenna-packaged ESP32-S3-WROOM-1 module. 

ESP32-S3 WROOM

.. image:: ../_static/imgs/Preface/Preface02.png
    :align: center

The hardware interfaces of ESP32-S3 WROOM are distributed as follows:

.. image:: ../_static/imgs/Preface/Preface03.png
    :align: center

Compare the left and right images. We've boxed off the resources on the ESP32-S3 WROOM in different colors to facilitate your understanding of the ESP32-S3 WROOM.

.. list-table:: 
   :width: 70%
   :header-rows: 1 
   :align: center
   :class: text-center table-line
   
   * -  Box color 
     -  Corresponding resources introduction

   * -  |Preface04|
     -  GPIO pins
     
   * -  |Preface05|
     -  LED indicators

   * -  |Preface06|
     -  Camera interface

   * -  |Preface07|
     -  Reset button, Boot mode selection button 

   * -  |Preface08|
     -  USB ports

.. |Preface04| image:: ../_static/imgs/Preface/Preface04.png
.. |Preface05| image:: ../_static/imgs/Preface/Preface05.png
.. |Preface06| image:: ../_static/imgs/Preface/Preface06.png
.. |Preface07| image:: ../_static/imgs/Preface/Preface07.png
.. |Preface08| image:: ../_static/imgs/Preface/Preface08.png

For more information, please visit: https://www.espressif.com.cn/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf. 

**GPIO pins of ESP32-S3 WROOM can be used to interface with external devices and control peripheral circuits.**

**In the following projects, we only use USB cable to power ESP32-S3 WROOM by default. But you can still power the ESP3-S3 WROOM Shield with external power supply, with a voltage range of 6-12V and current of over 0.5A.**

Notes for GPIO
**************************

PSRAM Pin
=======================

The module on the ESP32-S3-WROOM board utilizes the ESP32-S3R8 chip, which comes with 8MB of external Flash. When using the OPI PSRAM, it should be noted that GPIO35-GPIO37 on the ESP32-S3-WROOM board will not be available for other purposes. However, when OPI PSRAM is not used, GPIO35-GPIO37 on the board can be used as normal GPIO.

.. image:: ../_static/imgs/Preface/Preface09.png
    :align: center

SDcard Pin
========================

An SDcard slot is integrated on the back of the ESP32-S3-WROOM board, and we can use GPIO38-GPIO40 of ESP32-S3-WROOM to drive SD card.

The SDcard of ESP32-S3-WROOM uses SDMMC, a 1-bit bus driving method, which is integrated in the Arduino IDE, and we can call the "SD_MMC.h" library to drive it. For more details, please refer to the SDcard chapter in this tutorial.

USB Pin
=======================

In Micropython, GPIO19 and GPIO20 are dedicated to the USB function of the ESP32S3 and cannot be used for other functions. 

Please note that in this product, GPIO19 is used to read the ADC value of the external power supply, and therefore should not be used for USB functions to avoid conflicts.

Cam Pin
=======================

When using the camera of our ESP32-S3 WROOM, please check the pins of it. Pins with underlined numbers are used by the camera function, if you want to use other functions besides it, please avoid using them.

.. image:: ../_static/imgs/Preface/Preface10.png
    :align: center

.. table::
    :align: center
    :class: text-center zebra
    :width: 50%
    
    +-----------+-----------+
    |  CAM_Pin  |  GPIO_pin |
    +===========+===========+
    | SIOD      |  GPIO4    |
    +-----------+-----------+
    | SIOC      |  GPIO5    |
    +-----------+-----------+
    | CSI_VYSNC |  GPIO6    |
    +-----------+-----------+
    | CSI_HREF  |  GPIO7    |
    +-----------+-----------+
    | CSI_Y9    |  GPIO16   |
    +-----------+-----------+
    | XCLK      |  GPIO15   |
    +-----------+-----------+
    | CSI_Y8    |  GPIO17   |
    +-----------+-----------+
    | CSI_Y7    |  GPIO18   |
    +-----------+-----------+
    | CSI_PCLK  |  GPIO13   |
    +-----------+-----------+
    | CSI_Y6    |  GPIO12   |
    +-----------+-----------+
    | CSI_Y2    |  GPIO11   |
    +-----------+-----------+
    | CSI_Y5    |  GPIO10   |
    +-----------+-----------+
    | CSI_Y3    |  GPIO9    |
    +-----------+-----------+
    | CSI_Y4    |  GPIO8    |
    +-----------+-----------+

If you have any questions regarding GPIO information, you can click :ref:`here <Preface>` to navigate back to the ESP32-S3 WROOM and view specific GPIO details.

Or check: https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf.

CH343 (Importance)
***********************************

ESP32-S3 WROOM uses CH343 to download code. Therefore, before using the device, it is necessary to install the CH343 driver on your computer.

Windows
====================================

Check whether CH343 has been installed
-----------------------------------------------

1.	Connect your computer and ESP32-S3 WROOM with a USB cable.

.. image:: ../_static/imgs/Preface/Preface11.png
    :align: center

2.	Turn to the main interface of your computer, select "This PC" and right-click to select "Manage".

.. image:: ../_static/imgs/Preface/Preface12.png
    :align: center

3.	Click "Device Manager". If your computer has installed CH343, you can see"USB-Enhances-SERIAL CH343 (COMx)". And you can click :ref:`here <Programming>` to move to the next step.

.. image:: ../_static/imgs/Preface/Preface13.png
    :align: center

Installing CH343
--------------------------

1.	First, download CH343 driver, click http://www.wch-ic.com/search?t=all&q=ch343 to download the appropriate one based on your operating system.

.. image:: ../_static/imgs/Preface/Preface14.png
    :align: center

If you would not like to download the installation package, you can open " **Freenove_Ultimate_Starter_Kit_for_ESP32_S3/CH343** ", we have prepared the installation package.

2.	Open the folder "Freenove_Ultimate_Starter_Kit_for_ESP32_S3/CH343/Windows/"

.. image:: ../_static/imgs/Preface/Preface15.png
    :align: center

3.	Double click " **CH343SER.EXE** ".

.. image:: ../_static/imgs/Preface/Preface16.png
    :align: center

4.	Click "INSTALL" and wait for the installation to complete.

.. image:: ../_static/imgs/Preface/Preface17.png
    :align: center

5.	Install successfully. Close all interfaces.

.. image:: ../_static/imgs/Preface/Preface18.png
    :align: center

6.	When ESP32-S3 WROOM is connected to computer, select "This PC", right-click to select "Manage" and click "Device Manager" in the newly pop-up dialog box, and you can see the following interface.

.. image:: ../_static/imgs/Preface/Preface19.png
    :align: center

7.	So far, CH343 has been installed successfully. Close all dialog boxes. 

MAC
===========================

First, download CH343 driver. Click http://www.wch-ic.com/search?t=all&q=ch343 to download the appropriate one based on your operating system.

.. image:: ../_static/imgs/Preface/Preface20.png
    :align: center

If you would not like to download the installation package, you can open " **Freenove_Ultimate_Starter_Kit_for_ESP32_S3/CH343** ". We have prepared the installation package.

Second, open the folder "Freenove_Ultimate_Starter_Kit_for_ESP32_S3/CH343/MAC/"

.. image:: ../_static/imgs/Preface/Preface21.png
    :align: center

Third, click Continue.

.. image:: ../_static/imgs/Preface/Preface22.png
    :align: center

Fourth, click Install.

.. image:: ../_static/imgs/Preface/Preface23.png
    :align: center

Then, waiting Finsh.

.. image:: ../_static/imgs/Preface/Preface24.png
    :align: center

Finally, restart your PC.

.. image:: ../_static/imgs/Preface/Preface25.png
    :align: center

If it fails to be installed with the above steps, you can refer to readme.pdf to install it. 

.. image:: ../_static/imgs/Preface/Preface26.png
    :align: center

.. _Programming:

Programming Software
*********************************

Arduino Software (IDE) is used to write and upload the code for Arduino Board.

First, install Arduino Software (IDE): visit https://www.arduino.cc/en/software/

.. image:: ../_static/imgs/ArduinoIDE/Preface03.png
    :align: center
    :width: 99%
    :class: image-border

Select and download corresponding installer based on your operating system. If you are a Windows user, please select the "Windows" to download and install the driver correctly.

.. image:: ../_static/imgs/ArduinoIDE/Preface04.png
    :align: center

After the downloading completes, run the installer. For Windows users, there may pop up an installation dialog box of driver during the installation process. When it is popped up, please allow the installation.

After installation is completed, an shortcut will be generated in the desktop.

.. image:: ../_static/imgs/ArduinoIDE/Preface05.png
    :align: center

Run it. The interface of the software is as follows:

.. image:: ../_static/imgs/ArduinoIDE/Preface06.png
    :align: center

Programs written with Arduino IDE are called sketches. These sketches are written in a text editor and are saved with the file extension.ino. The editor has features for cutting/pasting and for searching/replacing text. The console displays text output by the Arduino IDE, including complete error messages and other information. The bottom right-hand corner of the window displays the configured board and serial port. The toolbar buttons allow you to verify and upload programs, open the serial monitor, and access the serial plotter.

.. table::
    :align: center
    :class: table-line
    :width: 80%
    
    +-------------+---------------------------------------------------------------------+
    | |Arduino07| | Verify                                                              |
    |             |                                                                     |
    |             | Checks your code for errors compiling it.                           |
    +-------------+---------------------------------------------------------------------+
    | |Arduino08| | Upload                                                              |
    |             |                                                                     |
    |             | Compiles your code and uploads it to the configured board.          |
    +-------------+---------------------------------------------------------------------+
    | |Arduino09| | Debug                                                               |
    |             |                                                                     |
    |             | Troubleshoot code errors and monitor program running status.        |
    +-------------+---------------------------------------------------------------------+
    | |Arduino10| | Serial Plotter                                                      |
    |             |                                                                     |
    |             | Real-time plotting of serial port data charts.                      |
    +-------------+---------------------------------------------------------------------+
    | |Arduino11| | Serial Monitor                                                      |
    |             |                                                                     |
    |             | Used for debugging and communication between devices and computers. |
    +-------------+---------------------------------------------------------------------+

.. |Arduino07| image:: ../_static/imgs/ArduinoIDE/Preface07.png
.. |Arduino08| image:: ../_static/imgs/ArduinoIDE/Preface08.png
.. |Arduino09| image:: ../_static/imgs/ArduinoIDE/Preface09.png
.. |Arduino10| image:: ../_static/imgs/ArduinoIDE/Preface10.png
.. |Arduino11| image:: ../_static/imgs/ArduinoIDE/Preface11.png

Environment Configuration
***********************************

First, open the software platform arduino, and then click File in Menus and select Preferences.

.. image:: ../_static/imgs/Preface/Preface37.png
    :align: center

Second, click on the symbol behind "Additional Boards Manager URLs" 

.. image:: ../_static/imgs/Preface/Preface38.png
    :align: center

Third, fill in https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json in the new window, click OK, and click OK on the Preferences window again.

.. image:: ../_static/imgs/Preface/Preface39.png
    :align: center

.. note::
    
    if you copy and paste the URL directly, you may lose the "-". Please check carefully to make sure the link is correct.

Fourth, click "Boards Manager". Enter "esp32" in Boards manager, select 2.0.7, and  click "INSTALL".

.. image:: ../_static/imgs/Preface/Preface40.png
    :align: center

Arduino will download these files automaticly. Wait for the installation to complete.

.. image:: ../_static/imgs/Preface/Preface41.png
    :align: center

When finishing installation, click Tools in the Menus again and select Board: "ESP32S3 Dev Module", and then you can see information of ESP32-S3. 

.. image:: ../_static/imgs/Preface/Preface42.png
    :align: center

Library Installation
*********************************

Before starting the learning process, it is necessary to install some libraries in advance to enable the code to be compiled properly. For convenience, we have already packaged these libraries and placed them in the Freenove Development Kit for ESP32-S3/Libraries folder. Please refer to the following steps to install these libraries into the Arduino IDE.

1.	Open Arduino IDE.

.. image:: ../_static/imgs/Preface/Preface43.png
    :align: center

2.	Select Sketch->Include Library->Add .ZIP library....

.. image:: ../_static/imgs/Preface/Preface46.png
    :align: center

3.	On the newly pop-up window, select the files from the Freenove-Development-Kit-for-ESP32-S3/Libraries. Click Open to install the library.

.. image:: ../_static/imgs/Preface/Preface44.png
    :align: center

4.	Repeat the above steps until all the six libraries are installed to Arduino. So far, all libraries have been installed.

.. note::
    
    Some libraries are not the latest version. Please do not update them even if it prompts every time you open the IDE. Just click LATER. Otherwise, it may lead the compilation to fail.

.. image:: ../_static/imgs/Preface/Preface45.png
    :align: center