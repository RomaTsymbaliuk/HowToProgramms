#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
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

	remote.sin_addr.s_addr = inet_addr(cl->interface);
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
	union u_data_frame *last_pkg = NULL;
	union u_data_frame **packages;
	int i = 0;
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

	printf("There will be %d packages to receive\n", num_packages);

	packages = malloc(sizeof(union u_frame*) * (num_packages));
	if (!packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}


	for (int z = 0; z < num_packages - 1; z++) {
		if (z == 0 && packet_id != FILE_UPLOAD) {
			one_package_size = TCP_LIMIT + file_name_size + file_name_path;
		} else {
			one_package_size = TCP_LIMIT;
		}
		packages[z] = malloc(one_package_size);
		if (!packages[z]) {
			printf("Memory corruption\n");
		}
		if ( (size = recv(cl->sockfd, (packages[z])->u_data, one_package_size, 0)) >= 0) {
		} else {
			return ERR_READ;
		}
	}

	last_pkg_size = packet_len % TCP_LIMIT;

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
		tcp_client_execute_handler(packages, last_pkg,
								   last_pkg_size, num_packages, cl);

		break;

	case FILE_UPLOAD:
		printf("FILE TO UPLOAD\n");
		tcp_client_upload_file_handler(packages, last_pkg, last_pkg_size, file_name_size,
									  file_name_path, num_packages, cl);

		break;
	}


	for (int j = 0; j < num_packages - 1; j++) {
		if (packages[j]) {
			free(packages[j]);
			packages[j] = NULL;
		}
	}

	if (last_pkg) {
		free(last_pkg);
		last_pkg = NULL;
	}

	if (packages) {
		free(packages);
		packages = NULL;
	}


	return SUCCESS;
}

int tcp_client_send_file_handler(union u_data_frame **packages, union u_data_frame *last_pkg, int file_name_size, int file_name_path, 
								 int last_pkg_size, int num_packages, struct client *cl)
{
	char path[1024];
	int sent_size = 0;
	union u_management_frame to_send_management;
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

	printf("NUM PACKAGES %d\n", num_packages);

	for (int z = 0; z < num_packages - 1; z++) {
		if (z == 0) {
			memcpy(cmd_data, (packages[z]->u_data + file_name_size + file_name_path), TCP_LIMIT);
		} else {
			memcpy(cmd_data, (packages[z]->u_data), TCP_LIMIT);
		}
		fwrite(cmd_data, TCP_LIMIT, 1, fp);
	}
	printf("\nasdadsasdaweq2131231232-3---213-123-12-xxxxxxxx-3------333333\n");

	if (last_pkg) {
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

	if (file_name) {
		free(file_name);
		file_name = NULL;
	}
	if (file_path) {
		free(file_path);
		file_path = NULL;
	}

	return SUCCESS;
}

int tcp_client_execute_handler(union u_data_frame **packages, union u_data_frame *last_pkg, 
							   int last_pkg_size, int num_packages, struct client *cl)
{
	unsigned char command[MAX_COMMAND_LEN] = {0};
	char *result;
	int num_send_packages;
	int z = 0;
	int i;
	int packet_len;
	int last_send_pkg_size;
	union u_data_frame **send_packages;
	union u_data_frame *last_send_pkg;
	union u_management_frame management_frame;

	if (num_packages * TCP_LIMIT > MAX_COMMAND_LEN) {
		printf("Sorry command is too long\n");
		return ERR_COMMAND;
	}

	printf("Im here or not\n");

	for (z; z < num_packages - 1; z++) {
		memcpy(command + z * TCP_LIMIT, packages[z]->u_data, TCP_LIMIT);
	}

	if (last_pkg) {
		memcpy(command + z * TCP_LIMIT, (last_pkg->u_data), last_pkg_size);
	}

	printf("COMMAND\n:%s\n", command);

	result = client_executor(command);

	if (result) {

		packet_len = strlen(result) * sizeof(char);

		num_send_packages = packet_len / TCP_LIMIT + 1;

		management_frame.packet_frame.packet_len = htonl(packet_len);
		management_frame.packet_frame.packet_id = htonl(COMMAND_EXECUTE);
		management_frame.packet_frame.file_name_size = htonl(0);
		management_frame.packet_frame.file_name_path_size = htonl(0);

		if (write(cl->sockfd, (void*)(management_frame.u_data), FRAME_LENGTH) < 0) {
			printf("Error write\n");
			return ERR_WRITE;
		}

		send_packages = malloc(sizeof(union u_data_frame*) * num_send_packages);
		if (!send_packages) {
			return MEMORY_ALLOCATION_ERROR;
		}
		for (i = 0; i < num_send_packages - 1; i++) {
			send_packages[i] = malloc(TCP_LIMIT);
			if (!send_packages[i]) {
				return MEMORY_ALLOCATION_ERROR;
			}
			memcpy(send_packages[i], result + i * TCP_LIMIT, TCP_LIMIT);
			if (write(cl->sockfd, (void*)(send_packages[i]->u_data), TCP_LIMIT) < 0) {
				printf("Error write\n");
				return ERR_WRITE;
			}
			if (send_packages[i]) {
				free(send_packages[i]);
				send_packages[i] = NULL;
			}

		}
		last_send_pkg_size = packet_len % TCP_LIMIT;
		if (last_send_pkg_size != 0) {
			last_send_pkg = malloc(last_send_pkg_size);
			if (!last_send_pkg)
				return MEMORY_ALLOCATION_ERROR;
			memcpy(last_send_pkg, result + i * TCP_LIMIT, last_send_pkg_size);
			if (write(cl->sockfd, (void*)(last_send_pkg->u_data), last_send_pkg_size) < 0) {
				printf("Error write\n");
				return ERR_WRITE;
			}
			if (last_send_pkg) {
				free(last_send_pkg);
				last_send_pkg = NULL;
			}
		}
	} else {

		management_frame.packet_frame.packet_len = htonl(0);
		management_frame.packet_frame.packet_id = htonl(COMMAND_EXECUTE);
		management_frame.packet_frame.file_name_size = htonl(0);
		management_frame.packet_frame.file_name_path_size = htonl(0);

		if (write(cl->sockfd, (void*)(management_frame.u_data), FRAME_LENGTH) < 0) {
			printf("Error write\n");
			return ERR_WRITE;
		}
	}

	printf("Lived here!\n");

	return SUCCESS;
}

int tcp_client_upload_file_handler(union u_data_frame **packages, union u_data_frame *last_pkg, int last_pkg_receive_size,
								  int file_name_size, int file_name_path, 
								  int num_packages, struct client *cl)
{
	char *path;
	int sent_size = 0;
	union u_management_frame to_send_management;
	char *file_name;
	char *file_path;
	FILE *fp;
	unsigned char cmd_data[TCP_LIMIT];
	int i = 0;
	int k = 0;
	union u_data_frame **to_send_packages;
	union u_data_frame *to_send_last_pkg;
	int num_packages_send;
	int last_pkg_send_size;
	int one_package_size;
	int nbytes;
	int size;

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
	path = malloc(file_name_path + file_name_size + 1);
	if (!path){
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("SOME PLEASENT FILE TO UPLOAD\n");

	for (int x = 0; x < num_packages - 1; x++) {
		if (x * TCP_LIMIT < file_name_path) {
			memcpy(file_path + x * TCP_LIMIT, packages[x]->u_data, TCP_LIMIT);
		} else {
			memcpy(file_name + i * TCP_LIMIT, packages[x]->u_data, TCP_LIMIT);
			i++;
		}
	}
	if (last_pkg) {
		printf("Got last pkg\n");
		memcpy(file_name, last_pkg->u_data, file_name_size);
		memcpy(file_path, last_pkg->u_data + file_name_size, file_name_path);
	}

	file_name[file_name_size - 1] = '\0';
	file_path[file_name_path - 1] = '\0';
	printf("File name %s\n", file_name);
	printf("File path %s\n", file_path);


	strcpy(path, file_path);
	path[file_name_path - 1] = '/';
	strcat(path, file_name);
	printf("PATH : %s\n", path);

	fp = fopen(path, "rb");
	if (!fp) {
		printf("No such file %s\n", path);
		return ERR_UPLOAD;
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

	to_send_management.packet_frame.packet_id = htonl(FILE_UPLOAD);
	to_send_management.packet_frame.packet_len = htonl(sent_size);
	to_send_management.packet_frame.file_name_size = htonl(file_name_size);
	to_send_management.packet_frame.file_name_path_size = htonl(file_name_path);

	if (write(cl->sockfd, (void*)(to_send_management.u_data), FRAME_LENGTH) != FRAME_LENGTH) {
			printf("Error writing to socket\n");
			return ERR_WRITE;
	}

	num_packages_send = sent_size / TCP_LIMIT + 1;

	printf("To send %d packages\n", num_packages_send);

	one_package_size = file_name_size + TCP_LIMIT;

	to_send_packages = malloc(sizeof(union u_data_frame*) * (num_packages_send));
	if (!to_send_packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("SIZE %d \n", sent_size);

	for (k; k < num_packages_send - 1; k++) {
		to_send_packages[k] = malloc(one_package_size);
		if (!to_send_packages[k]) {
			printf("Corrupted at k is %d\n", k);
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		fread(cmd_data, TCP_LIMIT, 1, fp);
		if (k == 0) {
			one_package_size = file_name_size + TCP_LIMIT;
			memcpy(to_send_packages[k]->packet_frame.cmd_data, file_name, file_name_size);
			memcpy((to_send_packages[k]->packet_frame.cmd_data + file_name_size), cmd_data, TCP_LIMIT);
		} else {
			memcpy((to_send_packages[k]->packet_frame.cmd_data), cmd_data, TCP_LIMIT);
			one_package_size = TCP_LIMIT;
		}
		if (write(cl->sockfd, (void*)(to_send_packages[k]->u_data), one_package_size) != one_package_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}
		if (to_send_packages[k]) {
			free(to_send_packages[k]);
			to_send_packages[k] = NULL;
		}
	}

	last_pkg_send_size = sent_size % TCP_LIMIT;
	printf("Last package size is %d\n", last_pkg_send_size);

	if (last_pkg_send_size) {
		printf("NEED TO SEND LAST PKG\n");
		printf("SENT SIZE %d\n", last_pkg_send_size);
		one_package_size = last_pkg_send_size;
		to_send_last_pkg = malloc(one_package_size);
		if (!to_send_last_pkg) {
			printf("Memory allocation\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		fread(cmd_data, last_pkg_send_size, 1, fp);
		memcpy(to_send_last_pkg->packet_frame.cmd_data, cmd_data, last_pkg_send_size);
		printf("SENT LAST PKG SIZE %d\n", one_package_size);
		printf("-----------22222----------\n");
		if (write(cl->sockfd, (void*)(to_send_last_pkg->u_data), last_pkg_send_size) != last_pkg_send_size) {
				printf("Error writing to socket\n");
				return ERR_WRITE;
		}
		if (to_send_last_pkg)
			free(to_send_last_pkg);
		to_send_last_pkg = NULL;
	}

	if (to_send_packages) {
		free(to_send_packages);
	}

	fclose(fp);

	return SUCCESS;
}