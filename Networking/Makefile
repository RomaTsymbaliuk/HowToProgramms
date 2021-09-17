CC=gcc
DC=cc
TCP=TCP
UDP=UDP
CLIENT=Client
ERRORS=Errors
USERSHELL=UserShell
SERIALIZE=Serialize
MENU=MENU
SHARED=SharedMemory
PACKET=Packet

main: Server/src/*.c Client/src/*.c $(ERRORS)/src/*.c $(USERSHELL)/src/*.c $(SHARED)/src/*.c $(PACKET)/src/*.c 
	$(CC) -o server -IServer/headers -I$(ERRORS)/headers -I$(USERSHELL)/headers -I$(SHARED)/headers -I$(PACKET)/headers -I$(MENU) Server/src/*.c $(USERSHELL)/src/*.c $(SHARED)/src/*.c $(PACKET)/src/*.c menu_objs.h
	$(CC) -o client -IClient/headers -I$(ERRORS)/headers -I$(PACKET)/headers Client/src/*.c 

debug: Server/src/*.c Client/src/*.c $(ERRORS)/src/*.c
	$(DC) -g -IServer/headers -I$(ERRORS)/headers -I$(USERSHELL)/headers Server/src/*.c $(USERSHELL)/src/*.c
