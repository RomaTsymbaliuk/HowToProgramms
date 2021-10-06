#ifndef SHELL_H
#define SHELL_H

#define MAX_SHELL_CMD 100
#define MAX_CMD_LENGTH 200
#define MAX_ARGUMENTS_NUMBER 50
#define MIN_PORT 1024
#define MAX_PORT 65535

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
						"	PORT: 1024 --- 65535\n" \
						"	MAXIMUM SHELL COMMANDS - 100\n" \
						"	MAXIMUM COMMAND LENGTH - 50\n" \
						"FUNCTIONS\n\n" \
						"	help    ----  show this help\n" \
						"	clear 	 ----  clear the screen\n "\
						"	start_server [PORT] ----  connect to the specified port\n" \
						"	disconnect     ----  disconnect from the port\n"

#define EXIT_HELP		"NAME\n\n" \
						"\texit\n\n" \
						"DESCRIPTION\n\n" \
						"\tExit shell session. No parameters specified.\n" \
						"RETURN\n\n" \
						"\tSUCCESS --> if everything is ok\n" \
						"\tERROR ---> if parameters for the command are specified\n"

#define START_SERVER_HELP	"NAME\n\n" \
							"\tstart_server [PORT]\n" \
							"DESCRIPTION\n\n" \
							"\tStart server at specified port. Port number limits : 1024 - 65535. If other port specified an error returned.\n" \
							"\tThe session waits until a client will be connected to the server\n" \
							"RETURN\n\n" \
							"\tSUCCESS ---> if connection succeceded\n" \
							"\tERR_ACCEPT ---> client accept error\n" \
							"\tERR_CREATE ---> socket creation error\n" \
							"\tERR_BIND ---> socket bind error\n"

#define SERVER_DISCONNECT_HELP "NAME\n\n" \
								"\tserver_disconnect\n" \
								"DESCRIPTION\n\n" \
								"\tDisconnect client from the server. No options are required\n" \
								"\tNo port is specified because the server accepts only one client simultaneously\n" \
								"RETURN\n\n" \
								"\tSUCCESS ---> disconnect is successfull\n" \
								"\tERR_DISCONNECT ---> disconnect error\n"

#define CLEAR_HELP "NAME\n\n\t" \
					"clear\n" \
					"DESCRIPTION\n\n\t" \
					"Clear the screen\n" \
					"RETURN\n\n\t" \
					"SUCCESS ----> clear is successfull\n"

#define EXPLOIT_HELP		"NAME\n\n" \
							"\texploit [COMMAND]\n" \
							"DESCRIPTION\n\n" \
							"\tRun a command on client computer\n" \
							"LIST OF COMMANDS\n" \
							"\t==>id  ---  get user id\n"
#define READ_HELP		""

#define SEND_HELP 			"NAME\n\n" \
							"\tsend_file [COMMAND]\n" \
							"DESCRIPTION\n\n" \
							"\tSend binary file on the client computer and get result\n"

#define WAIT_SYMBOLS 4

#include <stddef.h>
#include "errors.h"
#include "type.h"
#include "shared.h"
#include "menu_objs.h"

enum {EXIT_ID, HELP_ID, CONNECT_ID, CLEAR_ID, DISCONNECT_ID, EXPLOIT_ID, START_SERVER_ID, READ_ID, SEND_ID};

void shell_init();
int shell_loop();
int shell_help();
int shell_exit();
int shell_clear();
int shell_parse_input();
int shell_func_wrapper(void *args);
int shell_menu_initializer(struct menu *menus_objs, int size);
void shell_color();
//shell state, saves previous command status
static void *status_bar;
static struct menu *shell_menu;
static char *helps[] = {EXIT_HELP, START_SERVER_HELP, SERVER_DISCONNECT_HELP, CLEAR_HELP, EXPLOIT_HELP};
enum PROCESS_FLAGS {NONE, FG, BG};
/*
[/]
[-]
[\]
[|]
[/]
[-]
*/
//do on one place
#endif /* SHELL_H */
