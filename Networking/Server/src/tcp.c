#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include "tcp.h"


int tcp_server_listen(struct server *server)
{
	printf("TCP listen\n");
	listen(server->sockfd, 3);
	return SUCCESS;
}

int tcp_server_bind(struct server *server)
{
	int iRetval = -1;
	struct sockaddr_in remote = {0};
	short sockfd;

	server->server_id = TCP;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return error(ERR_CREATE_SOCKET);
	}
	server->sockfd = sockfd;
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = htonl(INADDR_ANY);
	remote.sin_port = htons(PORT);
	iRetval = bind(server->sockfd, (struct sockaddr *)&remote, sizeof(remote));
	if (iRetval == -1) {
		return error(ERR_BIND);
	}

	return SUCCESS;
}

int tcp_server_read(struct server *server)
{

	char buff[200];
	int num = -1;
	bzero(buff, 200);
	while (strlen(buff) == 0) {
		bzero(buff, 200);
		read(server->sockfd, buff, sizeof(buff));
		printf("GOT : %s\n", buff);
	}
	printf("buff: %s\n", buff);
	return SUCCESS;
}

int tcp_server_accept(struct server *server)
{
	struct sockaddr_in cli;
	int len;
	int sock;

	len = sizeof(cli);
	sock = accept(server->sockfd, (SA*)&cli, &len);

	return sock;
}