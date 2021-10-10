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
	void *recv_input;
	union u_frame *pkg;
	uint32_t packet_id;
	uint32_t file_name_size;
	uint32_t file_name_path;
	uint32_t cmd_size;
	uint32_t packet_len;
	char *cmd_data;
	char *file_name;
	char *file_path;
	char *result;
	int i = 0;
	int start_parse;
	int size_to_receive;
	int size;

	recv_input = malloc(FRAME_LENGTH);
	if (!recv_input) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	if( (size = recv(server_object->sockfd, recv_input, FRAME_LENGTH, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}
	packet_len = ntohl(*((uint32_t*)recv_input));
	packet_id = ntohl(*((uint32_t*)(recv_input + 1 * sizeof(uint32_t))));
	file_name_size = ntohl(*((uint32_t*)(recv_input + 2 * sizeof(uint32_t))));
	file_name_path = ntohl(*((uint32_t*)(recv_input + 3 * sizeof(uint32_t))));

/*
	printf("PACKET LEN : %d\n", packet_len);
	printf("PACKET ID IS : %d\n", packet_id);
	printf("FILE NAME SIZE : %d\n", file_name_size);
	printf("FILE NAME PATH SIZE : %d\n", file_name_path);
*/

	size_to_receive = packet_len + FRAME_LENGTH + file_name_size + file_name_path;

	pkg = malloc(size_to_receive);
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	if (packet_len < 0 || packet_len > 100000) {
		printf("Invalid packet len %d\n", packet_len);
		return MEMORY_ALLOCATION_ERROR;
	}

	if ( (size = recv(server_object->sockfd, pkg->u_data, size_to_receive , 0)) >= 0) {
	} else {
		printf("Receive DATA error\n");
	}

//	for (int k = 0 ; k < packet_len + file_name_path + file_name_size + FRAME_LENGTH; k++) {
//		printf("BYTE %d hex %x char %c\n", k, (pkg->u_data)[k], (pkg->u_data)[k]);
//}
	switch(packet_id) {

	case COMMAND_EXECUTE:
		cmd_data = malloc(packet_len);
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		memcpy(cmd_data, pkg->u_data, packet_len);
		cmd_data[packet_len - 1] = '\0';
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$\n");
		printf("\n%s\n", cmd_data);
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$\n");
		free(cmd_data);
		break;

	case FILE_EXECUTE:
		printf("FILE SENT\n");
		break;

	case FILE_UPLOAD:
		file_name = malloc(file_name_size);
		if (!file_name) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		memcpy(file_name, pkg->u_data + FRAME_LENGTH, file_name_size);
		printf("Uploaded file %s\n", file_name);
		FILE *f = fopen(file_name, "wb");
		if (f) {
			fwrite((pkg->packet_frame.cmd_data + file_name_size), packet_len , 1, f);
		}
		else {
			printf("Error opening\n");
		}
		fclose(f);
		free(file_name);
		break;

	default:
		printf("Unrecognized packet id\n");
		break;
	}

	free(recv_input);

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
	union u_frame *pkg;
	char **args;
	char *dyn_args;
	int i = 0;
	int k = 0;
	int arg_size_counter = 0;
	int nbytes;
	int sent_size;
	uint32_t cmd_size;

	nbytes = 0;
	args = (char**)input->args;
	while(args[i] != NULL) {
		printf("ARG %d : %s\n",i, args[i]);
		arg_size_counter = arg_size_counter + strlen(args[i]);
		i++;
	}
	//size of the arguments + spaces between arguments
	arg_size_counter += i * sizeof(char);
	dyn_args = (char*)malloc(sizeof(char) * arg_size_counter);

	for (i = 0; args[i] != NULL; i++) {
		for (int j = 0; j < strlen(args[i]); j++) {
			dyn_args[k] = args[i][j];
			k++;
		}
		dyn_args[k] = ' ';
		k++;
	}

	printf("\nARGS PASSED : %s\n", dyn_args);

	dyn_args[strlen(dyn_args) - 2] = '\0';
	cmd_size = arg_size_counter * sizeof(char);
	pkg = malloc(cmd_size + FRAME_LENGTH);
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	pkg->packet_frame.packet_len = htonl(cmd_size);
	pkg->packet_frame.packet_id = htonl(COMMAND_EXECUTE);
	pkg->packet_frame.file_name_size = htonl(0);
	pkg->packet_frame.file_name_path_size = htonl(0);

	sent_size = cmd_size + FRAME_LENGTH;

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	pkg->packet_frame.packet_len = htonl(0);
	pkg->packet_frame.packet_id = htonl(0);
	pkg->packet_frame.file_name_size = htonl(0);
	pkg->packet_frame.file_name_path_size = htonl(0);

	memcpy(pkg->packet_frame.cmd_data, dyn_args, cmd_size);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), sent_size, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != sent_size) {
			printf("Error writing to socket\n");
			free(dyn_args);
			free(pkg);
			return ERR_WRITE;
	}

	free(dyn_args);
	free(pkg);

	return SUCCESS;
}

int tcp_server_send_file(struct menu *input)
{
	union u_frame *pkg;
	char **args;
	char *dyn_args;
	unsigned char *cmd_data;
	int sent_size;
	int nbytes = 0;
	size_t size = 0;
	FILE *fp;

	nbytes = 0;

	args = (char**)input->args;

	args[0][strlen(args[0])] = '\0';
	args[1][strlen(args[1]) - 1] = '\0';

	printf("ARG 0 %s ARG 1 %s\n", args[0], args[1]);

	fp = fopen(args[0], "rb");
	if (fp == NULL) {
		printf("File not exists\n");
		return ERR_READ;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("\nFILE SIZE %d\n", size);
	if (size <= 0) {
		printf("HEY. FILE SIZE IS 0\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	cmd_data = malloc(size);
	if (!cmd_data) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	fread(cmd_data, size, 1, fp);

	fclose(fp);

	sent_size = size + FRAME_LENGTH + strlen(args[0]) + strlen(args[1]);

	pkg = malloc(sent_size);
	if (!pkg) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	pkg->packet_frame.packet_id = htonl(FILE_EXECUTE);
	pkg->packet_frame.packet_len = htonl(size);
	memcpy(pkg->packet_frame.cmd_data, args[0], strlen(args[0]));
	memcpy((pkg->packet_frame.cmd_data + strlen(args[0])), args[1], strlen(args[1]));
	memcpy((pkg->packet_frame.cmd_data + strlen(args[0]) + strlen(args[1])), cmd_data, size);
	pkg->packet_frame.file_name_size = htonl(strlen(args[0]));
	pkg->packet_frame.file_name_path_size = htonl(strlen(args[1]));

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), sent_size, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != sent_size) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	free(pkg);
	free(cmd_data);

	return SUCCESS;
}

int tcp_server_upload(struct menu *input)
{
	int nbytes;
	int sent_size;
	union u_frame *pkg;
	union u_frame *frame;
	char **args;
	FILE *fp;
	size_t size = 0;

	args = (char**)input->args;

	sent_size = FRAME_LENGTH + strlen(args[0]);
	pkg = malloc(sent_size);
	if (!pkg) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	pkg->packet_frame.packet_id = htonl(FILE_UPLOAD);
	pkg->packet_frame.packet_len = htonl(strlen(args[0]));
	pkg->packet_frame.file_name_path_size = htonl(0);
	pkg->packet_frame.file_name_size = htonl(0);

	memcpy(pkg->packet_frame.cmd_data, args[0], (strlen(args[0]) * sizeof(char)));

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), sent_size, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != sent_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	tcp_server_read();

	free(pkg);

	return SUCCESS;
}