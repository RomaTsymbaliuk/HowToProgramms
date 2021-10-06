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

#include <stdlib.h>
#include <stdio.h>
#include "Server.h"

int server_exploit(struct menu *input)
{
	if (server_object->server_write(input) != SUCCESS) {
		return ERR_EXPLOIT;
	}
	if (server_object->server_read(input) != SUCCESS) {
		return ERR_READ;
	}

	return SUCCESS;
}

int server_connect(struct menu *input)
{
	int port = atoi(input->args[0]);

	if (server_object->server_init(port) != SUCCESS) {
		return ERR_INIT;
	}

	return SUCCESS;
}

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

int server_send(struct menu *input)
{
	if (server_object->server_send(input) != SUCCESS) {
		return ERR_SEND;
	}
}