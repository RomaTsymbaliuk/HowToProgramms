#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/mman.h>
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
	{"exit", EXIT_HELP, shell_exit, NULL, EXIT_ID, 0, NONE},
	{"start_server", START_SERVER_HELP, server_connect, NULL, START_SERVER_ID, 1, WAIT},
	{"help", SHELL_HELP, shell_help, NULL, HELP_ID, 0, NONE},
	{"server_disconect", SERVER_DISCONNECT_HELP, server_disconnect, NULL, DISCONNECT_ID, 0, NONE},
	{"clear", CLEAR_HELP, shell_clear, NULL, CLEAR_ID, 0, NONE},
	{NULL, NULL, NULL, NULL, 0, 0, NONE} // Sdelat norm
};

int server_connect(struct menu *input)
{
	int port = atoi(input->args[0]);
	printf("\nadssasdlkadsaklsdlasdkads\n");
	printf("\nPORT : %d\n", port);
	server_object->server_init(port);
}

int server_disconnect(struct menu *input)
{
	server_object->server_disconnect();
}

void *create_shared_memory(size_t size)
{
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANONYMOUS;

	return mmap(NULL, size, protection, visibility, -1, 0);
}

struct server *server_initialize(struct server *object)
{
	void *p = create_shared_memory(sizeof(object));
	memcpy(p, object ,sizeof(object));
	p = (struct server*)p;
	printf("\nALLOCATED AT %p\n" ,(void*)p);

	return p;
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
		server_object = server_initialize(&tcp_obj);
		memcpy(server_object, &tcp_obj, sizeof(tcp_obj));
		server_object = (struct server *)server_object;
		//server_object = &tcp_obj;
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
