@ECHO Off
cd firmware
download_all_sb.bat edbg ATSAM4SD32C Tools\serial_bridge\sam4s_xplained_pro_serial_bridge.elf SAM4S 0x00400000 0x00200000
pause