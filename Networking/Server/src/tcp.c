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
	if (port < MIN_PORT || port > MAX_PORT) {
		printf("Port number to small or to big\n");
		return ERR_INIT;
	}
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

	return SUCCESS;
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
	int size;
	void *recv_input;
	uint32_t packet_id;
	uint32_t packet_len;
	uint32_t cmd_size;
	char *cmd_data;

	if( (size = recv(server_object->sockfd, recv_input, sizeof(union u_frame), 0)) >= 0) {
		packet_id = ntohl((((union u_frame*)recv_input)->pkt.fields).cmd_id);
		cmd_size = ntohl((((union u_frame*)recv_input)->pkt.fields).cmd_len);
		cmd_data = ((((union u_frame*)recv_input)->pkt.fields).cmd_data);
		printf("HERE3\n");
		printf("CMD_SIZE : %d\n", cmd_size);
		printf("PACKET_ID : %d\n", packet_id);
		printf("DATA : %s\n", cmd_data);
		return SUCCESS;
	} else {
		printf("Receive error\n");
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
	union u_frame *pkg;
	uint32_t pkg_id;
	uint32_t pkg_len;
	uint32_t pkg_process_flags;
	uint32_t cmd_id;
	uint32_t cmd_size;
	char pkg_args[100] = { 0 };
	char **args;
	char args_to_send[1024];
	int i;

	nbytes = 0;

	args = (char**)input->args;

	for (i = 0; args[i] != NULL; i++) {
		printf("%s\n", args[i]);
		strcat(args_to_send, args[i]);
		strcat(args_to_send, " ");
	}

	pkg_id = htonl(TCP_ID);
	cmd_id = htonl(cmd_id);
	pkg_len = htonl(sizeof(pkg));
	cmd_size = htonl(50);

	pkg = malloc(sizeof(union u_frame));

	printf("HERE1\n");
	printf("args to send : %s %d\n", args_to_send, sizeof(args_to_send));
	printf("HERE2 %s \n;", args_to_send);

//	pkg->pkt.fields.cmd_data = strdup(args_to_send);

	memcpy(&(pkg->pkt.header.packet_id), &pkg_id, sizeof(uint32_t));
	memcpy(&(pkg->pkt.header.packet_len), &pkg_len, sizeof(uint32_t));
	memcpy(&(pkg->pkt.fields.cmd_len), &cmd_size, sizeof(uint32_t));
	memcpy(pkg->pkt.fields.cmd_data, args_to_send, sizeof(args_to_send));

	if ((nbytes = sendto(server_object->sockfd, (void*)pkg, sizeof(*pkg), 0, (struct sockaddr*)&remote, sizeof(remote))) != sizeof(union u_frame)) {
		printf("Error writing to socket\n");
		return ERR_WRITE;
	}

	tcp_server_read();


	return SUCCESS;
}
