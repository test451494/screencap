#
#
#
#
CC = /home/n320/arm926t_fpu_443/output/staging/usr/bin/arm-linux-uclibcgnueabi-gcc

DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -Os -pipe -O2 --sysroot /home/n320/sdk/output/staging/ -O2 -Wall -W -D_REENTRANT $(DEFINES)
#INC = -I./include
#LIB = -lpthread -ljpeg
INCPATH       = -I../../../../nashoba_nordica5_qt_ubifs_ext/output/staging/usr/mkspecs/qws/linux-arm-g++ -I. -I../../../../nashoba_nordica5_qt_ubifs_ext/output/staging/usr/include/QtCore -I../../../../nashoba_nordica5_qt_ubifs_ext/output/staging/usr/include/QtNetwork -I../../../../nashoba_nordica5_qt_ubifs_ext/output/staging/usr/include/QtGui -I../../../../nashoba_nordica5_qt_ubifs_ext/output/staging/usr/include -I. -Iutil -Ilanguage -Inetwork -Iui -Inetwork/arp -Iaudio -Ivideo -Iinclude -Icomm -Idhcp -Iui/main -Iui/util -Iui/other -Iui/talk -Iui/camera -Iui/security -Iui/message -Iui/user -Iui/system -Iinclude/jpeg -I.
LINK          = /home/n320/arm926t_fpu_443/output/staging/usr/bin/arm-linux-uclibcgnueabi-g++
LFLAGS        = --sysroot /home/n320/sdk/output/staging/ -Wl,-O1
LIBS          =  -L/home/n320/sdk/output/staging/usr/lib -lQtGui -L/home/n320/sdk/output/staging/usr/lib -ldirectfb -lfusion -ldirect -lQtNetwork -lQtCore -lpthread -lasound -ljpeg -liconv ./lib/cxon2dec-6.0.0.a ./lib/lib_dc_gfx.a ./lib/libdcapi.a ./lib/libcmem.a ./lib/libYUVScale.a

screencap: screencap.o 
	${LINK} ${LFLAGS} -o ./bin/screencap screencap.o ${LIBS}

program.o: program.c 
	${CC} -c ${CFLAGS} ${INCPATH} screencap.c


clean:
	rm -rf screencap.o
