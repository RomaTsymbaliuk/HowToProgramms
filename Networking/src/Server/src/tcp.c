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
	uint32_t packet_id;
	uint32_t file_name_size;
	uint32_t file_name_path;
	int size;
	uint32_t cmd_size;
	char *cmd_data;
	char *file_name;
	char *file_path;
	char *result;
	union u_frame *pkg;
	uint32_t packet_len;
	int i = 0;
	int start_parse;

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

	printf("PACKET LEN : %d\n", packet_len);
	printf("PACKET ID IS : %d\n", packet_id);
	printf("FILE NAME SIZE : %d\n", file_name_size);
	printf("FILE NAME PATH SIZE : %d\n", file_name_path);

	pkg = malloc(packet_len + FRAME_LENGTH + file_name_size + file_name_path);
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	pkg->packet_frame.packet_len = packet_len;
	pkg->packet_frame.packet_id = packet_id;
	pkg->packet_frame.file_name_size = file_name_size;
	pkg->packet_frame.file_name_path_size = file_name_path;
	if (packet_len < 0 || packet_len > 100000) {
		printf("Invalid packet len %d\n", packet_len);
		return MEMORY_ALLOCATION_ERROR;
	}
	printf("\n-------------------------111--------------------------\n");

	if ( (size = recv(server_object->sockfd, pkg->packet_frame.cmd_data, packet_len + file_name_path + file_name_size, 0)) >= 0) {
	} else {
		printf("Receive DATA error\n");
	}

	if (file_name_size == 0) {
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$\n");
		printf("\n%s\n", pkg->packet_frame.cmd_data);
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$\n");
	} else {
		file_name = malloc(file_name_size);
		if (!file_name) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		file_path = malloc(file_name_path);
		if (!file_path) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		result = malloc((packet_len - file_name_size - file_name_path));
		if (!result) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("Try to distuinguish\n");
		memcpy(file_name, pkg->packet_frame.cmd_data, file_name_size);
		memcpy(file_path, (pkg->packet_frame.cmd_data + file_name_size), file_name_path);
		memcpy(result, (pkg->packet_frame.cmd_data + file_name_size + file_name_path), packet_len);
		file_name[file_name_size - 1] = '\0';
		file_path[file_name_path] = '\0';

		printf("FILE %s at %s executed\nResult : \n%s", file_name, file_path, result);

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
	int arg_size_counter = 0;

	nbytes = 0;

	args = (char**)input->args;
	while(args[i] != NULL) {
		printf("ARG %d : %s\n",i, args[i]);
		arg_size_counter = arg_size_counter + strlen(args[i]);
		i++;
	}
	//size of the arguments + spaces between arguments
	arg_size_counter += i * sizeof(char);
	printf("ARG-SIZE-COUNTER %d\n", arg_size_counter);
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
//	cmd_size = arg_len * sizeof(char);
	dyn_args[strlen(dyn_args) - 2] = '\0';
	cmd_size = arg_size_counter * sizeof(char);
	printf("TO SEND %d cmd size \n ", cmd_size);
	pkg = malloc(cmd_size + FRAME_LENGTH);
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	pkg->packet_frame.packet_len = htonl(cmd_size);
	pkg->packet_frame.packet_id = htonl(COMMAND_EXECUTE);
	pkg->packet_frame.file_name_size = htonl(0);
	pkg->packet_frame.file_name_path_size = htonl(0);

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

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), (cmd_size + FRAME_LENGTH), 0,
		(struct sockaddr*)&remote, sizeof(remote))) != ( cmd_size + FRAME_LENGTH )) {
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
	int nbytes = 0;
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
	size_t size = 0;
	FILE *fp;

	nbytes = 0;

	args = (char**)input->args;

	args[0][strlen(args[0])] = '\0';
	args[1][strlen(args[1]) - 1] = '\0';

	fp = fopen(args[0], "rb");
	if (fp == NULL) {
		printf("File not exists\n");
		return ERR_READ;
	}

	if (fp) {

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
	}

	printf("SIZE IS %d\n", size);
	pkg = malloc(size + FRAME_LENGTH + strlen(args[0]) + strlen(args[1]));
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

	printf("\n-------------------------111 SEND FILE--------------------------\n");

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), (FRAME_LENGTH + size + strlen(args[0]) + strlen(args[1])), 0,
		(struct sockaddr*)&remote, sizeof(remote))) != (size + strlen(args[0]) + strlen(args[1]) + FRAME_LENGTH)) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	printf("\n-------------------------111 SEND FILE ENDED--------------------------\n");


	return SUCCESS;
}

int tcp_server_upload(struct menu *input)
{
	int nbytes;
	union u_frame *pkg;
	union u_frame *frame;
	void *recv_file_upload;
	uint32_t pkg_id;
	uint32_t pkg_len;
	uint32_t cmd_id;
	uint32_t cmd_size;
	char **args;
	char *dyn_args;
	unsigned char *cmd_data;
	int file_size;
	char c;
	FILE *fp;
	size_t size = 0;

	args = (char**)input->args;
	dyn_args = malloc(sizeof(char) * strlen(args[0]));
	if (!dyn_args) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	strcpy(dyn_args, args[0]);
	dyn_args[strlen(dyn_args) - 1] = '\0';

	cmd_size = strlen(dyn_args);

	printf("ARG : %s size %d", dyn_args, cmd_size);

	pkg = malloc(cmd_size + 2 * sizeof(uint32_t));
	if (!pkg) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	pkg->packet_frame.packet_id = htonl(FILE_UPLOAD);
	pkg->packet_frame.packet_len = htonl(cmd_size);
	for (int z = 0; z < cmd_size; z++) {
		printf("%c\n", dyn_args[z]);
	}
	memcpy(pkg->packet_frame.cmd_data, dyn_args, cmd_size);

	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), 8, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != 8) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}
/*
	printf("CMD-SIZE %d\n", cmd_size);

	printf("------------------------CHECKING------------------\n");
	for (int y = 0; y < cmd_size + 2 * sizeof(uint32_t); y++) {
		printf("Byte %d hex %x char %c\n", y, ((char*)(pkg->u_data)[y]), ((char*)(pkg->u_data)[y]));
	}
*/
	if ((nbytes = sendto(server_object->sockfd, (void*)(pkg->u_data), cmd_size + 2 * sizeof(uint32_t), 0,
		(struct sockaddr*)&remote, sizeof(remote))) != cmd_size + 2 * sizeof(uint32_t)) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	read(server_object->sockfd, &pkg_len, 4);
	pkg_len = ntohl(pkg_len);
	printf("PACKET_LEN FILE : %d\n", pkg_len);
	read(server_object->sockfd, &pkg_id, 4);
	pkg_id = ntohl(pkg_id);
	printf("PACKET ID FILE : %d\n", pkg_id);

	recv_file_upload = malloc(pkg_len);
	if (!recv_file_upload) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	if( (size = read(server_object->sockfd, recv_file_upload, pkg_len)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}
	file_size = pkg_len - 2 * sizeof(uint32_t);
	printf("UPLOAD FILE SIZE %d\n", file_size);
	cmd_data = malloc(file_size);
	if (!cmd_data) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	memcpy(cmd_data, (recv_file_upload), file_size);
	printf("Written in cmd_data %d bytes\n", file_size);

	fp = fopen("file_received", "wb");
	if (fp == NULL) {
		printf("File not exists\n");
		return ERR_READ;
	}

	fwrite(cmd_data, file_size, 1, fp);

	fclose(fp);

	printf("File closed\n");

	free(frame);
	free(dyn_args);
	free(pkg);

	return SUCCESS;
}