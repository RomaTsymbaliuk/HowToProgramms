#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "tcp.h"


int tcp_server_listen(struct server *server)
{
	printf("TCP listen\n");
	return SUCCESS;
}

int tcp_server_accept(struct server *server)
{
	printf("TCP accept\n");
	return SUCCESS;
}

int tcp_server_bind(struct server *server)
{
	printf("TCP bind\n");
	return SUCCESS;
}