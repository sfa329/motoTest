::删除Keil编译产生的一些垃圾文件
::删除Code Warrior编译产生的一些垃圾文件
del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
::del *.opt /s  ::不允许删除JTAG的设置
del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s
del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s
del *._2i /s
del *.L2P /s
del *.FED /s


del *.elf /s
del *.args /s
del *.mk /s
del *.local /s

exit