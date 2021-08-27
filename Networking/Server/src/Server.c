#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "Server.h"

/* ------ HELP ------- */
/*
	NAME
							main - server - client reverse shell programm
						SYNOPSIS
							./main [OPTIONS]
						DESCRIPTION

						It is a shell help. Enter command and arguments for the command. Special commands are writtend below

									--tcp			----->			create server with tcp connection
									--udp			----->			create server with udp connection
									--ntp			----->			create server with ntp connection
									--dns			----->			create server with dns connection
									--help			----->			show this help

						EXAMPLE
							>>>connect 8080
							>>>help
						SPECIAL COMMANDS
							>>>exit     ------    to exit the shell
							>>>cmd?     ------    to show this help
						LIMITS
								MAXIMUM SHELL COMMANDS - 100
								MAXIMUM COMMAND LENGTH - 50
						FUNCTIONS
								help    ----  show this help
								clear 	 ----  clear the screen
								connect [PORT] ----  connect to the specified port
*/

struct menu menus_objs[SHELL_CMD_NUM] = {
	{"exit", EXIT_ID, shell_exit, NULL, 0, NONE},
	{"start_server", START_SERVER_ID, server_connect, NULL, 1, WAIT},
	{"help", HELP_ID, shell_help, NULL, 0, NONE},
	{"server_disconect", DISCONNECT_ID, server_disconnect, NULL, 0, NONE},
	{"clear", CLEAR_ID, shell_clear, 0, NONE},
	{"END", 0, NULL, NULL, 0, NONE}
};

int server_connect(struct menu *input)
{
	int port = atoi(input->args[0]);
	printf("PORT : %d\n", port);
	server_object->server_init(port);
}

int server_disconnect(struct menu *input)
{
	server_object->server_disconnect();
}

int main(int argc, char *argv[])
{
	int choice;
	int c;
	int size;
	int option_index;
	static struct option long_options[] = 
	{
		{"tcp", no_argument, 0, 't'},
		{"udp", no_argument, 0, 'u'},
		{"ntp", no_argument, 0, 'n'},
		{"dns", no_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	option_index = 0;
	c = getopt_long(argc, argv, "p:", long_options, &option_index);
	if (c == -1) {
		return ERR_OPTION;
	}
	switch(c) {
	case 't':
		server_object = &tcp_obj;
		break;
	case 'u':
		break;
	case 'n':
		break;
	case 'd':
		break;
	case 'h':
		printf(SERVER_HELP);
		return ERR_OPTION;
	case '?':
	default:
		printf(SERVER_HELP);
		printf("\n\nNo options specified. Abort\n\n");
		return ERR_OPTION;
	}
	shell_init();
	return shell_loop();
}
