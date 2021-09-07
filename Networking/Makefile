CC=gcc
DC=cc
TCP=TCP
UDP=UDP
CLIENT=Client
ERRORS=Errors
USERSHELL=UserShell
SERIALIZE=Serialize
MENU=Menu
SHARED=SharedMemory
PACKET=Packet

main: Server/src/*.c Client/src/*.c $(ERRORS)/src/*.c $(USERSHELL)/src/*.c $(SERIALIZE)/src/*.c $(SHARED)/src/*.c $(PACKET)/src/*.c
	$(CC) -o server -IServer/headers -I$(ERRORS)/headers -I$(USERSHELL)/headers -I$(SERIALIZE)/headers -I$(MENU) -I$(SHARED)/headers -I$(PACKET)/headers Server/src/*.c $(USERSHELL)/src/*.c $(SERIALIZE)/src/*.c $(SHARED)/src/*.c $(PACKET)/src/*.c
	$(CC) -o client -IClient/headers -I$(ERRORS)/headers Client/src/*.c 

debug: Server/src/*.c Client/src/*.c $(ERRORS)/src/*.c
	$(DC) -g -IServer/headers -I$(ERRORS)/headers -I$(USERSHELL)/headers Server/src/*.c $(USERSHELL)/src/*.c
