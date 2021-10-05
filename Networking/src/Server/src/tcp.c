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
	void *recv_from;
	uint32_t packet_id;
	uint32_t comd_id;
	int size;
	uint32_t cmd_size;
	char *cmd_data;
	char *result;
	union u_frame *frame;
	uint32_t packet_len;
	int i = 0;
	int start_parse;

	//cast to struct 
	//payload after struct header -> (alligned) 
	//read in massive in union in u_data and s union in struct and net to host
	read(server_object->sockfd, &packet_len, 4);
	packet_len = ntohl(packet_len);
	printf("PACKET_LEN : %d\n", packet_len);

	frame = malloc(sizeof(union u_frame));
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	if( (size = read(server_object->sockfd, frame->u_data, packet_len)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}
	cmd_size = ntohl(*((uint32_t*)(frame->u_data + 2 * sizeof(uint32_t))));
	start_parse = 3 * sizeof(uint32_t);
	printf("start_parse : %d\n", start_parse);
	for (int i = start_parse; i < 3 * sizeof(uint32_t) + cmd_size; i++) {
		printf("%c", frame->u_data[i]);
	}

	free(frame);


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
	union u_frame pkg;
	uint32_t pkg_id;
	uint32_t pkg_len;
	uint32_t pkg_process_flags;
	uint32_t cmd_id;
	uint32_t cmd_size;
	char **args;
	char *dyn_args;
	int i;
	int structures_size;
	int args_num;
	int arg_len = 0;
	int k = 0;
	int cmd_to_send = 0;

	nbytes = 0;

	args = (char**)input->args;

	dyn_args = (char*)malloc(sizeof(char) * arg_len);

	for (i = 0; args[i] != NULL; i++) {
		for (int j = 0; j < strlen(args[i]); j++) {
			dyn_args[k] = args[i][j];
			k++;
		}
		dyn_args[k] = ' ';
		k++;
	}

	printf("IN ARG ARRAY : %d\n", k);
	arg_len = k;
	printf("\nARGS PASSED : %s\n", dyn_args);

	cmd_size = arg_len * sizeof(char);
	printf("TO SEND %d cmd size \n ", cmd_size);
	pkg.packet_frame.packet_len = htonl(cmd_size);
	pkg.packet_frame.packet_id = htonl(0);
	pkg.packet_frame.cmd_len = htonl(cmd_size);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg.u_data), 4, 0,
			(struct sockaddr*)&remote, sizeof(remote))) != 4) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	pkg.packet_frame.packet_len = htonl(0);
	pkg.packet_frame.packet_id = htonl(0);
	pkg.packet_frame.cmd_len = htonl(0);
	memcpy(pkg.packet_frame.cmd_data, dyn_args, cmd_size);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg.u_data), (cmd_size + 15), 0,
			(struct sockaddr*)&remote, sizeof(remote))) != (cmd_size + 15) ) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	return SUCCESS;
}
