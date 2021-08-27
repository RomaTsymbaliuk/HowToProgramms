#ifndef SERVER_H
#define SERVER_H

#include "shell.h"
#include "type.h"
#include "tcp.h"

# define SERVER_HELP     "NAME\n" \
						"        main - server - client reverse shell programm\n" \
						"SYNOPSIS\n" \
						"        ./main \n" \
						"DESCRIPTION\n" \
						"        \n" \
						"It is a shell help. Enter command and arguments for the command. Special commands are writtend below\n" \
						"\n" \
						"			--tcp				------>								create server with tcp connection\n" \
						"			--udp				------>								create server with udp connection\n" \
						"			--ntp				------>								create server with ntp connection\n" \
						"			--dns				------>								create server with dns connection\n" \
						"			--help				------>								show this help\n" \
						"\n" \
						"EXAMPLE\n" \
						"         >>>connect 8080\n" \
						"         >>>help\n" \
						"\n" \
						"\n" \
						"SPECIAL COMMANDS\n\n" \
						"         exit     ------    to exit the shell\n" \
						"         cmd?     ------    to show this help\n" \
						"LIMITS\n\n" \
						"		PORT: 1024 --- 65535\n" \
						"		MAXIMUM SHELL COMMANDS - 100\n" \
						"		MAXIMUM COMMAND LENGTH - 50\n" \
						"FUNCTIONS\n\n" \
						"		help    ----  show this help\n" \
						"		clear 	 ----  clear the screen\n "\
						"		connect [PORT] ----  connect to the specified port" \
						"		disconnect     ----  disconnect from the port"

enum PROCESS_FLAGS {NONE, WAIT};

int server_connect(struct menu *input);
int server_disconnect(struct menu *input);

#endif /* __SERVER_H__ */