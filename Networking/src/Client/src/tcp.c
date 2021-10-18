#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <errno.h>
#include "tcp.h"
#include "executor.h"

int tcp_client_connect(struct client *cl, int port)
{
	short sockfd;
	int iRetval = -1;
	struct sockaddr_in remote = {0};
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return ERR_CREATE_SOCKET;
	}

	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);

	printf("\nConnecting on port : %d\n", port);

	iRetval = connect(sockfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
	cl->sockfd = sockfd;
	if (iRetval == -1) {
		return ERR_CONNECT;
	}

	return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
	uint32_t packet_id;
	uint32_t file_name_size;
	uint32_t file_name_path;
	uint32_t packet_len;
	union u_management_frame management_frame;
	union u_data_frame *last_pkg;
	union u_data_frame **packages;
	int i = 0;
	int j = 0;
	int size_to_receive;
	int num_packages = 1;
	int one_package_size = 0;
	int packet_size_copy = 0;
	int nbytes;
	int last_pkg_size = 0;
	int size;

//	printf("Entered here socket : %d\n", cl->sockfd);

	if( (size = recv(cl->sockfd, management_frame.u_data, FRAME_LENGTH, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}

	packet_len = ntohl(management_frame.packet_frame.packet_len);
	packet_id = ntohl(management_frame.packet_frame.packet_id);
	file_name_size = ntohl(management_frame.packet_frame.file_name_size);
	file_name_path = ntohl(management_frame.packet_frame.file_name_path_size);
	size_to_receive = packet_len + file_name_path + file_name_size ;
	packet_size_copy = packet_len;

	printf("PACKET LEN : %d\n", packet_len);
	printf("PACKET ID IS : %d\n", packet_id);
	printf("FILE NAME SIZE : %d\n", file_name_size);
	printf("FILE NAME PATH SIZE : %d\n", file_name_path);
	printf("Size to receive %d\n", size_to_receive);

	if (packet_len < 0) {
		printf("Invalid packet len %d\n", packet_len);
		return MEMORY_ALLOCATION_ERROR;
	}

	num_packages = (packet_len / TCP_LIMIT) + 1;

	one_package_size = file_name_size + file_name_path + TCP_LIMIT;

	size_to_receive = packet_len;
	packet_size_copy = packet_len;

	printf("There will be %d packages to receive\n", num_packages);

	packages = malloc(sizeof(union u_frame*) * (num_packages + 1));
	if (!packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	for (packet_size_copy; packet_size_copy > 0; packet_size_copy = packet_size_copy - TCP_LIMIT) {
		if (j == 0) {
			one_package_size = TCP_LIMIT + file_name_size + file_name_path;
		} else {
			one_package_size = TCP_LIMIT;
		}
		packages[j] = malloc(one_package_size);
		if (!packages[j]) {
			printf("Memory corruption\n");
		}
		if ( (size = recv(cl->sockfd, (packages[j])->u_data, one_package_size, 0)) >= 0) {
		} else {
			return ERR_READ;
		}
		j++;
	}

	one_package_size = (packet_size_copy + TCP_LIMIT);
	last_pkg_size = TCP_LIMIT + packet_size_copy;
	printf("LAST PKG SIZE %d\n", last_pkg_size);

	if ((last_pkg_size % TCP_LIMIT) != 0) {
		one_package_size = last_pkg_size;
		last_pkg = malloc(one_package_size);
		if (!last_pkg) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		printf("RECEIVED LAST PKG SIZE : %d\n", one_package_size);
		if ( (size = recv(cl->sockfd, (last_pkg)->u_data, one_package_size, 0)) >= 0) {
		} else {
				printf("Receive DATA error\n");
		}
	}

	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

	switch(packet_id) {

	case FILE_EXECUTE:
		tcp_client_send_file_handler(packages, last_pkg, file_name_size, 
									 file_name_path, last_pkg_size, num_packages, cl);
		break;

	case COMMAND_EXECUTE:
//		tcp_client_execute_handler(packet_len, num_packages, file_name_size,
//								   file_name_path, cl, pkg, packages);
		break;

	case FILE_UPLOAD:
//		tcp_client_upload_file_handler(packages, last_pkg, last_pkg_size, 
//									   num_packages, cl);
		break;
	}

	return SUCCESS;
}

int tcp_client_send_file_handler(union u_data_frame **packages, union u_data_frame *last_pkg, int file_name_size, int file_name_path, 
								 int last_pkg_size, int num_packages, struct client *cl)
{
	char path[1024];
	int sent_size = 0;
	union u_management_frame to_send_management;
	union u_data_frame *to_send_data;
	char *file_name;
	char *file_path;
	FILE *fp;
	unsigned char cmd_data[TCP_LIMIT];
	int i = 0;

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

	memcpy(file_name, (packages[0]->u_data), file_name_size);
	memcpy(file_path, (packages[0]->u_data + file_name_size), file_name_path);

	file_name[file_name_size] = '\0';
	file_path[file_name_path] = '\0';

	strcpy(path, file_path);
	strcat(path, "/");
	strcat(path, file_name);

	printf("NEW PATH TO OPEN %s\n", path);
	printf("file_name ---->>%s<<---\n", file_name);
	printf("file_path ---->>%s<<---\n", file_path);

	fp = fopen(path, "wb");
	if (!fp) {
		printf("File opening problem\n");
		return ERR_READ;
	}

	for (int z = 0; z < num_packages - 1; z++) {
		if (z == 0) {
			memcpy(cmd_data, (packages[z]->u_data + file_name_size + file_name_path), TCP_LIMIT);
		} else {
			memcpy(cmd_data, (packages[z]->u_data), TCP_LIMIT);
		}
		fwrite(cmd_data, TCP_LIMIT, 1, fp);
	}
	printf("\nasdadsasdaweq2131231232-3---213-123-12-xxxxxxxx-3------333333\n");

	if (last_pkg_size % TCP_LIMIT) {
		memcpy(cmd_data, (last_pkg->u_data), last_pkg_size);
		fwrite(cmd_data, last_pkg_size, 1, fp);
	}


	to_send_management.packet_frame.packet_id = htonl(FILE_EXECUTE);
	to_send_management.packet_frame.packet_len = htonl(0);
	to_send_management.packet_frame.file_name_size = htonl(strlen(file_name));
	to_send_management.packet_frame.file_name_path_size = htonl(strlen(file_path));

	if (write(cl->sockfd, (void*)(to_send_management.u_data), FRAME_LENGTH) < 0) {
		printf("Error write\n");
		return ERR_WRITE;
	}

	fclose(fp);
	free(file_name);
	free(file_path);

	return SUCCESS;
}
/*
int tcp_client_execute_handler(int packet_len, int num_packages, int file_name_size, int file_name_path,
							   struct client *cl, union u_frame *pkg, union u_frame **packages)
{
	char *cmd_data;
	char *result;
	int sent_size = 0;
	union u_frame *to_send;

	cmd_data = malloc(packet_len);
	if (!cmd_data) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	for (int z = 0; z < num_packages - 1; z++) {
		memcpy(cmd_data + (z * TCP_LIMIT), (packages[z]->u_data + FRAME_LENGTH), TCP_LIMIT);
	}

	result = client_executor(cmd_data);

	sent_size = strlen(result) + FRAME_LENGTH;

	if (sent_size > TCP_LIMIT * 100000) {
		printf("Too much to send\n");
		return SUCCESS;
	}

	to_send = malloc(sent_size);
	if (!to_send) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	to_send->packet_frame.packet_id = htonl(COMMAND_EXECUTE);
	to_send->packet_frame.packet_len = htonl(strlen(result));
	to_send->packet_frame.file_name_size = htonl(0);
	to_send->packet_frame.file_name_path_size = htonl(0);

	memcpy(to_send->packet_frame.cmd_data, result, strlen(result));

	if (write(cl->sockfd, (void*)(to_send->u_data), sent_size) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}

	free(to_send);
	free(cmd_data);
	free(result);

	return SUCCESS;
}

int tcp_client_upload_file_handler(union u_frame **packages, union u_frame *last_pkg, 
								   int last_pkg_size, int num_packages, struct client *cl)
{
	char *file_name_split;
	char *cmd_data;
	int size;
	int sent_size;
	union u_frame *to_send;
	char cmd_filename[TCP_LIMIT];
	char file_name_copy[1024];
	FILE *fp;

	printf("GOT UPLOAD\n");

	strncpy(cmd_filename, packages[0]->u_data + FRAME_LENGTH, TCP_LIMIT);

	for (int z = 1; z < num_packages - 1; z++) {
		strncat(cmd_filename, packages[z]->u_data + FRAME_LENGTH, strlen((packages[z]->u_data + FRAME_LENGTH)));
	}

	printf("\nCMD_FILENAME IN RESULT %s \n", cmd_filename);
	/*
	memcpy(cmd_filename, (pkg->packet_frame.cmd_data), packet_len);

	cmd_filename[packet_len - 1] = '\0';
	printf("(%s)\n", cmd_filename);
	fp = fopen(cmd_filename, "rb");
	if (fp == NULL) {
		printf("File not exists\n");
		return ERR_READ;
	}
	printf("cmd_filename %s \n", cmd_filename);

	//Fail if path is not absolute

	file_name_split = strtok(cmd_filename, "/");
	while(file_name_split != NULL) {
		strcpy(file_name_copy, file_name_split);
		file_name_split = strtok(NULL, "/");
	}

	printf("FINAL : %s\n", file_name_copy);

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("\nFILE SIZE %d\n", size);

/*
	cmd_data = malloc(size);
	if (!cmd_data) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	fread(cmd_data, size, 1, fp);
	fclose(fp);
	printf("cmd_data size : %d\n", strlen(cmd_data));

	sent_size = FRAME_LENGTH + size + strlen(file_name_copy);

	to_send = malloc(sent_size);
	if (!to_send) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	to_send->packet_frame.packet_len = htonl(size);
	to_send->packet_frame.packet_id = htonl(FILE_UPLOAD);
	to_send->packet_frame.file_name_path_size = htonl(0);
	printf("To write filename %s\n", file_name_copy);

	to_send->packet_frame.file_name_size = htonl(strlen(file_name_copy));
	memcpy(to_send->packet_frame.cmd_data, file_name_copy, strlen(file_name_copy));
	memcpy((to_send->packet_frame.cmd_data + strlen(file_name_copy)), cmd_data, size);

	if (write(cl->sockfd, (void*)(to_send->u_data), FRAME_LENGTH) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}

	printf("SENT FRAME LENGTH\n");

	if (write(cl->sockfd, (void*)(to_send->u_data), sent_size) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}
	printf("SENT BINARY FILE\n");

	free(cmd_data);
	free(to_send);
	free(cmd_filename);
	free(file_name_split);


	return SUCCESS;
}

*/