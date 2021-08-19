CC=gcc
DC=cc
SERVER=Server
CLIENT=Client
ERRORS=Errors
USERSHELL=UserShell

main: $(SERVER)/src/*.c $(CLIENT)/src/*.c $(ERRORS)/src/*.c main.c 
	$(CC) -o main -I$(SERVER)/headers -I$(CLIENT)/headers -I$(ERRORS)/headers -I$(USERSHELL)/headers $(SERVER)/src/*.c $(CLIENT)/src/*.c $(USERSHELL)/src/*.c  main.c 
