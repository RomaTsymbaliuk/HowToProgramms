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
	read(server_object->sockfd, &packet_id, 4);
	packet_id = ntohl(packet_id);
	printf("PACKET ID : %d\n", packet_id);

	frame = malloc(sizeof(union u_frame));
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	if( (size = read(server_object->sockfd, frame->u_data, packet_len)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}

	start_parse = 0;
	printf("start_parse : %d\n", start_parse);
	for (int i = start_parse; i < packet_len; i++) {
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
	union u_frame *pkg;
	uint32_t pkg_id;
	uint32_t pkg_len;
	uint32_t pkg_process_flags;
	uint32_t cmd_id;
	uint32_t cmd_size;
	char **args;
	char *dyn_args;
	int i = 0;
	int structures_size;
	int args_num;
	int arg_len = 0;
	int k = 0;
	int cmd_to_send = 0;

	nbytes = 0;

	args = (char**)input->args;
	while(args[i] != NULL) {
		printf("ARG %d : %s\n",i, args[i]);
		i++;
	}
	arg_len = (i + 1);
	printf("ARG-LEN %d\n", arg_len);
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

	cmd_size = strlen(dyn_args);
	printf("TO SEND %d cmd size \n ", cmd_size);
	pkg = malloc(cmd_size + 2 * sizeof(uint32_t));
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	pkg->packet_frame.packet_len = htonl(cmd_size);
	pkg->packet_frame.packet_id = htonl(COMMAND_EXECUTE);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), 8, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != 8) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	pkg->packet_frame.packet_len = htonl(0);
	pkg->packet_frame.packet_id = htonl(0);
	memcpy(pkg->packet_frame.cmd_data, dyn_args, cmd_size);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), (cmd_size + 2 * sizeof(uint32_t)), 0,
		(struct sockaddr*)&remote, sizeof(remote))) != (cmd_size + 2 * sizeof(uint32_t)) ) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	return SUCCESS;
}

int tcp_server_send_file(struct menu *input)
{
	int nbytes;
	union u_frame *pkg;
	uint32_t pkg_id;
	uint32_t pkg_len;
	uint32_t pkg_process_flags;
	uint32_t cmd_id;
	uint32_t cmd_size;
	char **args;
	char *dyn_args;
	int i = 0;
	int structures_size;
	int args_num;
	int arg_len = 0;
	int k = 0;
	int cmd_to_send = 0;
	int count = 0;
	unsigned char *cmd_data;
	char c;
	int byte_number = 0;
	FILE *fp;

	nbytes = 0;

	args = (char**)input->args;
	while(args[i] != NULL) {
		printf("ARG %d : %s\n",i, args[i]);
		i++;
	}
	arg_len = (i + 1);
	printf("ARG-LEN %d\n", arg_len);
	dyn_args = (char*)malloc(sizeof(char) * arg_len);

	for (i = 0; args[i] != NULL; i++) {
		for (int j = 0; j < strlen(args[i]); j++) {
			dyn_args[k] = args[i][j];
			k++;
		}
		dyn_args[k] = ' ';
		k++;
	}

	printf("\nARGS PASSED : %s\n", dyn_args);

	cmd_size = strlen(dyn_args);

	fp = fopen("/home/rtsymbaliuk/Desktop/Trainee/Networking/server", "rb");
	if (fp == NULL) {
		printf("File not exists\n");
		return ERR_READ;
	}

	if (fp) {

		while ((c = getc(fp)) != EOF) {
			byte_number++;
		}

		fseek(fp, 0, SEEK_SET);
		cmd_data = malloc(byte_number);
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("FILE LENGTH : %d\n", byte_number);
		i = 0;
		while ((c = getc(fp)) != EOF) {
			cmd_data[i++] = c;
		}

		fclose(fp);
	}

	pkg = malloc(byte_number + 2 * sizeof(uint32_t));
	if (!pkg) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("DATA READ\n%d\n", byte_number);
	pkg->packet_frame.packet_id = htonl(FILE_EXECUTE);
	pkg->packet_frame.packet_len = htonl(byte_number);
	memcpy(pkg->packet_frame.cmd_data, cmd_data, byte_number);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), 8, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != 8) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), byte_number, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != byte_number) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	free(pkg);

	return SUCCESS;
}