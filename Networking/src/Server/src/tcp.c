#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
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

	printf("INTERFACE : %s\n", server_object->interface);

	server_object->sockfd = sockfd;
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr(server_object->interface);
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
	union u_management_frame management_frame;
	uint32_t packet_id;
	uint32_t file_name_size;
	uint32_t file_name_path;
	uint32_t cmd_size;
	uint32_t packet_len;
	char *cmd_data;
	char *file_name;
	char *file_path;
	char *result;
	unsigned char cmd_result[4096] = {0};
	int size;
	int num_packages;
	int last_pkg_size;
	int i = 0;
	FILE *fp;

	if( (size = recv(server_object->sockfd, management_frame.u_data, FRAME_LENGTH, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}
	packet_len = ntohl(management_frame.packet_frame.packet_len);
	packet_id = ntohl(management_frame.packet_frame.packet_id);
	file_name_size = ntohl(management_frame.packet_frame.file_name_size);
	file_name_path = ntohl(management_frame.packet_frame.file_name_path_size);
	printf("Received file_name_size %d\n", file_name_size);
	printf("Received file_name_path %d\n", file_name_path);
	if (file_name_size > sizeof(cmd_result)) {
		printf("Too much length of cmd_result!!\n");
		return ERR_READ;
	}
//	for (int k = 0 ; k < packet_len + file_name_path + file_name_size + FRAME_LENGTH; k++) {
//		printf("BYTE %d hex %x char %c\n", k, (pkg->u_data)[k], (pkg->u_data)[k]);
//}
	switch(packet_id) {

		case COMMAND_EXECUTE:
			printf("Hi man! You want to send me reply\n");
			if (packet_len != 0) {
				num_packages = packet_len / TCP_LIMIT + 1;
				for (i; i < num_packages - 1; i++) {
					if( (size = recv(server_object->sockfd, cmd_result + i * TCP_LIMIT, TCP_LIMIT, 0)) >= 0) {
					} else {
						printf("Receive SIZE error\n");
					}
				}
				last_pkg_size = packet_len % TCP_LIMIT;
				if (last_pkg_size != 0) {
					if( (size = recv(server_object->sockfd, cmd_result + i * TCP_LIMIT, last_pkg_size, 0)) >= 0) {
					} else {
						printf("Receive SIZE error\n");
					}
				}
				printf("RESULT CMD\n%s", cmd_result);
			} else {
				printf("On client executor problem occured!\n");
			}

			break;

		case FILE_EXECUTE:

			printf("FILE SENT!!!!!!!\n");

			break;

		case FILE_UPLOAD:
			file_name = malloc(file_name_size);
			if (!file_name) {
				return MEMORY_ALLOCATION_ERROR;
			}
			num_packages = packet_len / TCP_LIMIT + 1;
			printf("FILE UPLOAD RECEIVE %d packages\n", num_packages);
			for (i; i < num_packages - 1; i++) {
					if (i == 0) {
						if( (size = recv(server_object->sockfd, cmd_result, file_name_size + TCP_LIMIT, 0)) >= 0) {
							memcpy(file_name, cmd_result, file_name_size);
							printf("FILE NAME : %s\n", file_name);
							fp = fopen(file_name, "wb");
							fwrite((cmd_result + file_name_size), 1, TCP_LIMIT, fp);
						} else {
							printf("Receive error!\n");
							return ERR_READ;
						}
					} else {
						if( (size = recv(server_object->sockfd, cmd_result, TCP_LIMIT, 0)) >= 0) {
							fwrite(cmd_result, 1, TCP_LIMIT, fp);
						} else {
							printf("Receive error!\n");
							return ERR_READ;
						}

					}
			}
			last_pkg_size = packet_len % TCP_LIMIT;
			if (last_pkg_size) {
				if( (size = recv(server_object->sockfd, cmd_result, last_pkg_size, 0)) >= 0) {
					fwrite(cmd_result, 1, last_pkg_size, fp);
				} else {
					printf("Receive SIZE error\n");
				}
			}
			fclose(fp);
			break;

		default:
			printf("Unrecognized packet id\n");
			break;
		}

	return SUCCESS;
}

int tcp_server_disconnect()
{
	sprintf(server_object->cli_state, ">>>");
	close(server_object->sockfd);

	return SUCCESS;
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
	union u_management_frame management_frame;
	union u_data_frame **packages;
	union u_data_frame *pkg;
	union u_data_frame *last_pkg;
	char **args;
	char *dyn_args;
	int i = 0;
	int k = 0;
	int arg_size_counter = 0;
	int nbytes;
	int sent_size;
	int num_packages;
	int length_to_copy;
	int one_package_size;
	int last_pkg_size;
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

	management_frame.packet_frame.packet_len = htonl(cmd_size);
	management_frame.packet_frame.packet_id = htonl(COMMAND_EXECUTE);
	management_frame.packet_frame.file_name_size = htonl(0);
	management_frame.packet_frame.file_name_path_size = htonl(0);

	if ((nbytes = sendto(server_object->sockfd, (void*)(management_frame.u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
	}

	num_packages = cmd_size / TCP_LIMIT + 1;

	printf("To send %d packages\n", num_packages);

	one_package_size = TCP_LIMIT;

	printf("TCP LIMIT %d\n", TCP_LIMIT);

	printf("One package size %d\n", one_package_size);

	packages = malloc(sizeof(union u_frame*) * (num_packages));
	if (!packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	length_to_copy = cmd_size;

	k = 0;

	printf("length_to_copy : %d\n", length_to_copy);

	if (length_to_copy > TCP_LIMIT) {
		for (int z = 0; z < num_packages - 1; z++) {
			packages[z] = malloc(one_package_size);
			if (!packages[z]) {
				printf("Memory corruption\n");
				return MEMORY_ALLOCATION_ERROR;
			}

			memcpy((packages[z]->packet_frame.cmd_data), dyn_args + z * TCP_LIMIT, one_package_size);

			if ((nbytes = sendto(server_object->sockfd, (void*)(packages[z]->u_data), one_package_size, 0,
				(struct sockaddr*)&remote, sizeof(remote))) != one_package_size) {
					printf("Error writing to socket\n");
					return ERR_WRITE;
			}
		}
	} else {
		printf("\nENTERERERED HERERERE\n");
	}

	last_pkg_size = cmd_size % TCP_LIMIT;
	printf("Last package size is %d\n", last_pkg_size);

	if ((last_pkg_size % TCP_LIMIT) != 0) {
		printf("NEED TO SEND LAST PKG\n");
		printf("SENT SIZE %d\n", last_pkg_size);
		one_package_size = last_pkg_size;
		last_pkg = malloc(one_package_size);
		if (!last_pkg) {
			printf("Memory allocation problem\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		memcpy(last_pkg->packet_frame.cmd_data, dyn_args + k * TCP_LIMIT, last_pkg_size);
		if ((nbytes = sendto(server_object->sockfd, (void*)(last_pkg->u_data), one_package_size, 0,
			(struct sockaddr*)&remote, sizeof(remote))) != one_package_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}
		if (last_pkg)
			free(last_pkg);
		last_pkg = NULL;
	}

	if (dyn_args)
		free(dyn_args);
	dyn_args = NULL;

	return SUCCESS;
}

int tcp_server_send_file(struct menu *input)
{
	union u_management_frame management_frame;
	union u_data_frame *last_pkg = NULL;
	union u_data_frame **packages;
	char **args;
	unsigned char cmd_data[TCP_LIMIT] = {0};
	int sent_size;
	int nbytes = 0;
	int num_packages = 0;
	int one_package_size = 0;
	int last_pkg_size = 0;
	int k = 0;
	size_t size = 0;
	FILE *fp;

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

	sent_size = size;

	management_frame.packet_frame.packet_id = htonl(FILE_EXECUTE);
	management_frame.packet_frame.packet_len = htonl(size);
	management_frame.packet_frame.file_name_size = htonl(strlen(args[0]));
	management_frame.packet_frame.file_name_path_size = htonl(strlen(args[1]));

	if ((nbytes = sendto(server_object->sockfd, (void*)(management_frame.u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	num_packages = sent_size / TCP_LIMIT + 1;

	printf("To send %d packages\n", num_packages);

	one_package_size = strlen(args[0]) + strlen(args[1]) + TCP_LIMIT;

	packages = malloc(sizeof(union u_frame*) * (num_packages));
	if (!packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("SIZE %d \n", sent_size);

	for (sent_size; sent_size > 0; sent_size = sent_size - TCP_LIMIT) {
		packages[k] = malloc(one_package_size);
		if (!packages[k]) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		fread(cmd_data, TCP_LIMIT, 1, fp);
		if (k == 0) {
			one_package_size = strlen(args[0]) + strlen(args[1]) + TCP_LIMIT;
			memcpy(packages[k]->packet_frame.cmd_data, args[0], strlen(args[0]));
			memcpy((packages[k]->packet_frame.cmd_data + strlen(args[0])), args[1], strlen(args[1]));
			memcpy((packages[k]->packet_frame.cmd_data + strlen(args[0]) + strlen(args[1])), cmd_data, TCP_LIMIT);
		} else {
			memcpy((packages[k]->packet_frame.cmd_data), cmd_data, TCP_LIMIT);
			one_package_size = TCP_LIMIT;
		}
		if ((nbytes = sendto(server_object->sockfd, (void*)(packages[k]->u_data), one_package_size, 0,
			(struct sockaddr*)&remote, sizeof(remote))) != one_package_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}
		if (packages[k]) {
			free(packages[k]);
			packages[k] = NULL;
		}
		k++;
	}

	last_pkg_size = sent_size + TCP_LIMIT;
	printf("Last package size is %d\n", last_pkg_size);

	if (last_pkg_size % TCP_LIMIT) {
		printf("NEED TO SEND LAST PKG\n");
		printf("SENT SIZE %d\n", last_pkg_size);
		one_package_size = last_pkg_size;
		last_pkg = malloc(one_package_size);
		if (!last_pkg) {
			printf("Memory allocation\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("-----------11111----------\n");
		one_package_size = last_pkg_size;
		fread(cmd_data, last_pkg_size, 1, fp);
		memcpy(last_pkg->packet_frame.cmd_data, cmd_data, last_pkg_size);
		printf("SENT LAST PKG SIZE %d\n", one_package_size);
		printf("-----------22222----------\n");
		if ((nbytes = sendto(server_object->sockfd, (void*)(last_pkg->u_data), one_package_size, 0,
			(struct sockaddr*)&remote, sizeof(remote))) != one_package_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}

		if (last_pkg)
			free(last_pkg);
		last_pkg = NULL;

	}

	if (packages) {
		free(packages);
	}

	fclose(fp);

	return SUCCESS;
}

int tcp_server_upload(struct menu *input)
{
	union u_management_frame management_frame;
	union u_data_frame *last_pkg = NULL;
	union u_data_frame **packages;
	union u_data_frame data_frame;
	char **args;
	char *token;
	char file_name[30];
	char *file_name_path;
	unsigned char cmd_data[TCP_LIMIT] = {0};
	int nbytes = 0;
	int num_packages = 0;
	int one_package_size = 0;
	int last_pkg_size = 0;
	int sent_size = 0;
	int file_name_size;
	int file_name_path_size;
	int k = 0;
	int i = 0;

	args = (char**)input->args;

	printf("entered her!!!e\n");

	args[0][strlen(args[0])] = '\0';
	file_name_path = strdup(args[0]);
	if (!file_name_path) {
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("ARG : %s \n",args[0]);

	sent_size = strlen(args[0]);

	printf("HHHHEEEERRREREE\n");

	token = strtok(args[0], "/");

	while(token != NULL) {
		printf("%s\n", token);
		memset(file_name, 0, sizeof(char) * 30);
		strncpy(file_name, token, strlen(token));
		token = strtok(NULL, "/");
	}

	file_name_size = strlen(file_name);
	file_name_path_size = sent_size - strlen(file_name);
	printf("FILENAME STRLEN : %s %d\n", file_name, file_name_size);
	file_name_path[file_name_path_size - 1] = '\0';
	printf("FILE PATH SIZE : %s %d\n", file_name_path, file_name_path_size);

	management_frame.packet_frame.packet_id = htonl(FILE_UPLOAD);
	management_frame.packet_frame.packet_len = htonl(sent_size);
	management_frame.packet_frame.file_name_size = htonl(file_name_size);
	management_frame.packet_frame.file_name_path_size = htonl(file_name_path_size);

	if ((nbytes = sendto(server_object->sockfd, (void*)(management_frame.u_data), FRAME_LENGTH, 0,
		(struct sockaddr*)&remote, sizeof(remote))) != FRAME_LENGTH) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	num_packages = sent_size / TCP_LIMIT + 1;

	printf("To send %d packages\n", num_packages);

	one_package_size = TCP_LIMIT;

	packages = malloc(sizeof(union u_frame*) * (num_packages));
	if (!packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("SIZE %d \n", sent_size);

	for (k = 0; k < num_packages - 1; k++) {
		packages[k] = malloc(one_package_size);
		if (!packages[k]) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("k->%d\n",k);
		if (k * TCP_LIMIT < file_name_path_size)
			memcpy((packages[k]->packet_frame.cmd_data), file_name_path + k * TCP_LIMIT, TCP_LIMIT);
		else {
			memcpy((packages[k]->packet_frame.cmd_data), file_name + i * TCP_LIMIT, TCP_LIMIT);
			i++;
		}

		if ((nbytes = sendto(server_object->sockfd, (void*)(packages[k]->u_data), one_package_size, 0,
			(struct sockaddr*)&remote, sizeof(remote))) != one_package_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}
		if (packages[k]) {
			free(packages[k]);
			packages[k] = NULL;
		}
	}

	last_pkg_size = sent_size % TCP_LIMIT;
	printf("Last package size is %d\n", last_pkg_size);

	if (last_pkg_size) {
		printf("NEED TO SEND LAST PKG\n");
		printf("SENT SIZE %d\n", last_pkg_size);
		one_package_size = last_pkg_size;
		last_pkg = malloc(one_package_size);
		if (!last_pkg) {
			printf("Memory allocation\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("-----------11111----------\n");
		one_package_size = last_pkg_size;
		memcpy(last_pkg->packet_frame.cmd_data, cmd_data, last_pkg_size);
		printf("SENT LAST PKG SIZE %d\n", one_package_size);
		printf("-----------22222----------\n");
		if ((nbytes = sendto(server_object->sockfd, (void*)(last_pkg->u_data), one_package_size, 0,
			(struct sockaddr*)&remote, sizeof(remote))) != one_package_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}

		if (last_pkg)
			free(last_pkg);
		last_pkg = NULL;

	}

	if (packages) {
		free(packages);
		packages = NULL;
	}
	if (file_name_path) {
		free(file_name_path);
		file_name_path = NULL;
	}

	tcp_server_read();

	return SUCCESS;
}