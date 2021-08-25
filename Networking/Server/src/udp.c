#include <stdio.h>
#include "udp.h"

int udp_server_listen(struct server *server)
{
	printf("UDP listen\n");
	return SUCCESS;
}

int udp_server_accept(struct server *server)
{
	printf("UDP accept\n");
	return SUCCESS;
}

int udp_server_bind(struct server *server)
{
	printf("UDP bind\n");
	return SUCCESS;
}