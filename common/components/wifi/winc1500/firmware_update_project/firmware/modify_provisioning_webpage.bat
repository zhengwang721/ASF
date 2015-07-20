@ECHO off
cd Tools\image_builder\debug\

:: Test path length.
if NOT "%CD:~230,1%"=="" (
	echo.
	echo [ERROR] File path is too long. Please move firmware update tool at the root of your hard drive and try again.
	echo.
	pause
	exit
)

image_builder -http_modify ../../../provisioning_webpage/ -fw_path ../../../firmware/m2m_aio_2b0.bin
image_builder -1003A0 -http_modify ../../../provisioning_webpage/ -fw_path ../../../firmware/m2m_aio_3a0.bin
