#ifndef SHELL_H
#define SHELL_H

#define MAX_SHELL_CMD 100
#define MAX_CMD_LENGTH 200
#define MAX_ARGUMENTS_NUMBER 50

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
						"EXAMPLE\n" \
						"         >>>ls /Desktop\n" \
						"         >>>ps aux \n" \
						"\n" \
						"\n" \
						"SPECIAL COMMANDS\n\n" \
						"         exit     ------    to exit the shell\n" \
						"         cmd?     ------    to show this help\n" \
						"LIMITS\n\n" \
						"        MAXIMUM SHELL COMMANDS - 100\n" \
						"        MAXIMUM COMMAND LENGTH - 50\n" \
						"FUNCTIONS\n\n" \
						"		 help    ----  show this help\n" \
						"		 clear 	 ----  clear the screen\n "\
						"		connect [PORT] ----  connect to the specified port"

#include <stddef.h>
#include "errors.h"
#include "tcp.h"

enum {EXIT_ID, HELP_ID, CONNECT_ID, CLEAR_ID};

struct menu {
	char *cmd_name;
	int cmd_id;
	char **args;
	int (*func)(void);
};

void shell_init(struct server *srv);
int shell_loop();
int shell_help();
int shell_exec();
int shell_exit();
int shell_connect();
int shell_clear();
struct menu *shell_parse_input();

static struct menu menus_objs[] = {
	{"exit", EXIT_ID, NULL, shell_exit},
	{"help", HELP_ID, NULL, shell_help},
	{"connect", CONNECT_ID, NULL, shell_connect},
	{"clear", CLEAR_ID, NULL, shell_clear}
};

//move to main , add help

static struct server *server_object;

/*
[/]
[-]
[\]
[|]
[/]
[-]
*/
#endif /* SHELL_H */
