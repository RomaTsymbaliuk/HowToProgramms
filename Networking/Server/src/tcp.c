#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include "tcp.h"

struct sockaddr_in remote = {0};

//proverka na port
int tcp_server_init(int port)
{
	if (tcp_server_bind(port) != SUCCESS) {
		printf("\nERROR BIND!\n");
		return ERR_BIND;
	}
	if (tcp_server_listen() != SUCCESS) {
		printf("\nERROR LISTEN\n");
		return ERR_LISTEN;
	}
	if (tcp_server_accept() != SUCCESS) {
		return ERR_ACCEPT;
	}
}
int tcp_server_listen()
{
	if (listen(server_object->sockfd, 8) < 0) {
		printf("\nListen error\n");
		return ERR_LISTEN;
	}
	return SUCCESS;
}

int tcp_server_bind(int port)
{
	int iRetval = -1;
	short sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return ERR_CREATE_SOCKET;
	}
	server_object->sockfd = sockfd;
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = htonl(INADDR_ANY);
	remote.sin_port = htons(port);
	server_object->port = port;
	iRetval = bind(server_object->sockfd, (struct sockaddr *)&remote, sizeof(remote));
	if (iRetval == -1) {
		return ERR_BIND;
	}

	return SUCCESS;
}

int tcp_server_read()
{
	char buff[200] = {0};
	int num = 0;
	bzero(buff, 200); // ?
	for(int i = 0; i < 20; i++) { //why 20??
		if (read(server_object->sockfd, buff, sizeof(buff)) < 0) {
			printf("Read error");
			return ERR_READ;
		}
		if (!strcmp(buff, "CONNECT")) {
			return SUCCESS;
		}
		bzero(buff, 200);
	}

	return SUCCESS;
}

int tcp_server_disconnect()
{
	sprintf(server_object->cli_state, ">>>");
	return 	close(server_object->sockfd);
}

int tcp_server_accept()
{
	struct sockaddr_in cli;
	int len;
	int sock;
	int *ptr;
	int wait_flag = 1;

	sock = accept(server_object->sockfd, (SA*)&cli, &len);
	server_object->sockfd = sock;

	return server_object->sockfd;
}

int tcp_server_write(struct menu *input)
{
	int nbytes;
	struct packet_frame pkg;
	uint32_t pkg_id;
	uint32_t pkg_len;
	uint32_t pkg_process_flags;
	uint32_t cmd_id;
	char pkg_args[100] = { 0 };
	char *args[] = {"Parameter 1", "Parameter 2", "Parameter 3", "Parameter 4"};
	nbytes = 0;
	cmd_id = input->cmd_id;

	pkg_id = htonl(TCP_ID);
	cmd_id = htonl(CMD_ID);
	printf("INPUT->cmd_id : %d \n", CMD_ID);

	pkg_len = htonl(sizeof(pkg));

	memcpy(&(pkg.header.packet_id), &pkg_id, sizeof(uint32_t));
	memcpy(&(pkg.header.packet_len), &pkg_len, sizeof(uint32_t));
	memcpy(&(pkg.cmd_id), &cmd_id, sizeof(cmd_id));
	memcpy(&(pkg.process_flag), &cmd_id, sizeof(pkg_process_flags));

	printf("PKG.CMD_ID : %d\n", pkg.cmd_id);

	if ((nbytes = sendto(server_object->sockfd, (void*)&pkg, sizeof(pkg), 0, (struct sockaddr*)&remote, sizeof(remote))) != sizeof(pkg)) {
		printf("Error writing to socket\n");
		return ERR_WRITE;
	}

	return SUCCESS;
}