/**
 * @file readme.txt
 *
 * @brief readme file
 *
 * $Id: readme.txt 33045 2012-09-13 08:48:05Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

 This bootloader is to upgrade the firmware in NCP using bootloader through a host controller.
                 _________________
                |  Bootloader PC  |  COM(USB)                EXT(USART) 
     			|	  tool        | ---------> Host Board ------------> NCP board
		        |_________________|

Supported devices: 
         MCU (NCP)
    atxmega256a3bu
    atmega256rfr2

Steps to use this bootloader:
1. Flash the corresponding device.hex. Example: For atxmega256a3u device, flash atxmega256a3u.hex 
   from the location "\thirdparty\wireless\avr2025_mac\addons\bootloader\xmega" 
2. In the host controller board,for example sam4l_xplained_pro. Flash the serial bridge application from the location 
   "\thirdparty\wireless\addons\serial_bridge\example\sam4lc4c_sam4l_xplained_pro"
3. Connect the NCP board, for example atxmega256a3u_zigbit_ext to "ext1" given in sam4l_xplained_pro (host).
   Install Bootloader_PC_Tool_Setup_1.2.2.235.exe from th location "\thirdparty\wireless\avr2025_mac\addons\bootloader"
4. Open the installed Bootloader PC tool, select the corresponding com port of the host board. 
5. Keep the serial port settings as
    Bit rate       9600
	Data bits      8
	Stop bits      1
	Parity         None
	HW control     None
6. Browse and select the .srec application which has to flashed in NCP.
7. Before the user clicks upload, reset button in NCP board should be pressed. When the user clicks upload, 
   he will be prompted as "please restart the device".Reset button can be released at this time and the upload will start.
8. Wait till the upload gets completed.   

		 