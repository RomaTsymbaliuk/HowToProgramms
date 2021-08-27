#ifndef SHELL_H
#define SHELL_H

#define MAX_SHELL_CMD 100
#define MAX_CMD_LENGTH 200
#define MAX_ARGUMENTS_NUMBER 50
#define MIN_PORT 1024
#define MAX_PORT 65535

#define SHELL_CMD_NUM 6

#define SHELL_INIT "**********************************************\n"   \
					"*********************************************\n"   \
					"***************   USER SHELL   **************\n"   \
					"*********************************************\n"   \
					"*********************************************\n"   \
					"***\t-Think twice before using it-\t***\n"    \
					"*********************************************\n"   \
					"*********************************************\n"   \

# define SHELL_HELP     "NAME\n" \
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

#include <stddef.h>
#include "errors.h"

enum {EXIT_ID, HELP_ID, CONNECT_ID, CLEAR_ID, DISCONNECT_ID, START_SERVER_ID};

struct menu {
	char *cmd_name;
	int cmd_id;
	void *func;
	void **args;
	int args_size;
	int process_flags;
};

void shell_init();
int shell_loop();
int shell_help();
int shell_exec();
int shell_exit();
int shell_clear();
struct menu *shell_parse_input();

//move to main , add help

extern struct menu menus_objs[SHELL_CMD_NUM];
/*
[/]
[-]
[\]
[|]
[/]
[-]
*/
#endif /* SHELL_H */
