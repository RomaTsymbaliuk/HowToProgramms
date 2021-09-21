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
	uint32_t packet_id;
	uint32_t comd_id;
	int size;
	uint32_t cmd_size;
	char cmd_data[1024];
	char *result;
	union u_frame frame;
	uint32_t tcp_size = 1024;
	uint32_t packet_len;
	int i = 0;

	//cast to struct 
	//payload after struct header -> (alligned) 
	//read in massive in union in u_data and s union in struct and net to host

	read(server_object->sockfd, &packet_len, sizeof(uint32_t));

	packet_len = ntohl(packet_len);
	printf("PACKET_LEN : %d\n", packet_len);

	if( (size = recv(server_object->sockfd, frame.u_data, packet_len, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}

	packet_id = ntohl(*((uint32_t*)(frame.u_data)));
	cmd_size = ntohl(*((uint32_t*)(frame.u_data + 2 * sizeof(uint32_t))));

	printf("cmd_size : %d\n", cmd_size);

/*
	cmd_data = malloc(cmd_size);
	if (!cmd_data) {
		return MEMORY_ALLOCATION_ERROR;
	}
*/
	
	for (int i = 0; i < packet_len; i++) {
		printf("Byte %d : %x", i, frame.u_data[i + 2 * sizeof(uint32_t)]);
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
	int structures_size;

	nbytes = 0;

	args = (char**)input->args;

	for (i = 0; args[i] != NULL; i++) {
		printf("%s\n", args[i]);
		strcat(args_to_send, args[i]);
		strcat(args_to_send, " ");
	}

	pkg_id = htonl(TCP_ID);
	cmd_id = htonl(cmd_id);
	pkg = malloc(sizeof(union u_frame));

	printf("HERE1\n");
	printf("args to send : %s %d\n", args_to_send, sizeof(args_to_send));
	printf("HERE2 %s \n;", args_to_send);

//	pkg->pkt.fields.cmd_data = strdup(args_to_send);

	cmd_size = strlen(args_to_send);
	structures_size = sizeof(struct packet_frame);
	memcpy(&(pkg->pkt.header.packet_id), &pkg_id, sizeof(uint32_t));
	memcpy(&(pkg->pkt.fields.cmd_len), &cmd_size, sizeof(uint32_t));
	memcpy(pkg->pkt.fields.cmd_data, args_to_send, sizeof(args_to_send));

	printf("cmd_size : %d structures_size : %d\n", cmd_size, structures_size);
	pkg_len = cmd_size + structures_size;
	pkg_len = htonl(pkg_len);
	memcpy(&(pkg->pkt.header.packet_len), &pkg_len, sizeof(uint32_t));
	for (int i = 0; i < strlen(args_to_send); i++) {
		printf("char - %c in hex : %x\n", args_to_send[i], args_to_send[i]);
	}
//send count elements in massive 12 + cmd
	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), structures_size + cmd_size, 0, 
		(struct sockaddr*)&remote, sizeof(remote))) != structures_size + cmd_size) {
		printf("Error writing to socket\n");
		return ERR_WRITE;
	}

	tcp_server_read();

	return SUCCESS;
}
