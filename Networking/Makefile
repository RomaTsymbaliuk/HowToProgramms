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

main: src/Server/src/*.c src/Client/src/*.c src/$(ERRORS)/src/*.c src/$(USERSHELL)/src/*.c src/$(PACKET)/src/*.c src/main.c 
	$(CC) -o server -Isrc/Server/headers -Isrc/$(ERRORS)/headers -Isrc/$(USERSHELL)/headers -Isrc/$(USERSHELL)/$(SHARED)/headers -Isrc/$(PACKET)/headers -Isrc/$(MENU) src/Server/src/*.c src/$(USERSHELL)/src/*.c src/$(PACKET)/src/*.c src/$(USERSHELL)/SharedMemory/src/* src/main.c
	$(CC) -o client -Isrc/Client/headers -Isrc/$(ERRORS)/headers -Isrc/$(PACKET)/headers src/Client/src/*.c 

debug: Server/src/*.c Client/src/*.c $(ERRORS)/src/*.c
	$(DC) -g -IServer/headers -I$(ERRORS)/headers -I$(USERSHELL)/headers Server/src/*.c $(USERSHELL)/src/*.c
