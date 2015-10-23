with b0 board, generated project will be run with below works.

1.Add below line in Project property -> Build Events -> Post-build event command line
"C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\SAMB11_DFP\1.0.75\tools\samb_makeimage.bat" "C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\SAMB11_DFP\1.0.75\patches" "$(OutputDirectory)\$(OutputFileName).bin"  app_entry

2. Add below line in Project property -> Tool -> External programming tool
"C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\SAMB11_DFP\1.0.75\tools\SambFlashTool.exe" write -i "$(OutputDirectory)\$(OutputFileName).img" -n $(avrtoolserialnumber) -c $(avrtoolinterfaceclock) -p $(avrdeviceexpectedsignature)

3. add "CHIPVERSION_B0" to Toolchain -> symbols -> Defined symbols

(optional) 4. add "debughooks.py" file to project root. 



----temp ----

"C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\SAMB11_DFP\1.0.82\tools\samb_makeimage.bat" "C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\SAMB11_DFP\1.0.82\patches" "$(OutputDirectory)\$(OutputFileName).bin"  app_entry
"C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\SAMB11_DFP\1.0.82\tools\SambFlashTool.exe" write -i "$(OutputDirectory)\$(OutputFileName).img" -n $(avrtoolserialnumber) -c $(avrtoolinterfaceclock) -p $(avrdeviceexpectedsignature)


"$(DEVICE_STARTUP_ROOT)\tools\samb_makeimage.bat" "$(DEVICE_STARTUP_ROOT)\patches" "$(OutputDirectory)\$(OutputFileName).bin"  app_entry

"$(DEVICE_STARTUP_ROOT)\tools\SambFlashTool.exe" write -i "$(OutputDirectory)\$(OutputFileName).img" -n $(avrtoolserialnumber) -c $(avrtoolinterfaceclock) -p $(avrdeviceexpectedsignature)





"$(DevEnvDir)\Packs\Atmel\SAMB11_DFP\1.0.75\tools\samb_makeimage.bat" "$(DevEnvDir)\Packs\Atmel\SAMB11_DFP\1.0.75\patches" "$(OutputDirectory)\$(OutputFileName).bin"  app_entry
