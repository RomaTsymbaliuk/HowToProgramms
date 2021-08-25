#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "tcp.h"
#include "errors.h"

int tcp_server_listen(struct tcp_server *server)
{
	int err = 0;

	err = (server->listen_fd = socket(AF_INET, SOCK_STREAM, 0));
	if (err == -1) {
		printf("Failed to create socket endpoint\n");
		return err;
	}
}

int tcp_server_accept(struct tcp_server *server)
{
	struct sockaddr_in server_addr = { 0 };
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	return SUCCESS;
}