CC=gcc
DC=cc
HEADERS=headers
SRC=src
main: $(SRC)/*.c
	$(CC) -o main -I$(HEADERS) $(SRC)/*.c

debug: $(SRC)/*.c
	$(DC) -g -I$(HEADERS) $(SRC)/*.c
clean:
	rm -rf main
	rm -rf a.out
