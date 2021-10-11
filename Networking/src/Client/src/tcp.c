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

/**
 * Accepts connections, handshakes with clients
 */
int tcp_client_send(struct client *cl, char *buff, int buff_len)
{
	union u_frame *frame;
	uint32_t packet_id;
	uint32_t packet_len;
	uint32_t cmd_len;
	uint32_t cmd_size;
	uint32_t packet_len_to_send;

	frame = malloc(buff_len + FRAME_LENGTH);
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	cmd_size = buff_len;

	packet_len = cmd_size + FRAME_LENGTH;
	packet_len_to_send = packet_len;

	packet_id = htonl(COMMAND_EXECUTE);
	packet_len = htonl(packet_len);

	memcpy(frame->packet_frame.cmd_data, buff, buff_len);
	memcpy(&(frame->packet_frame.packet_len), &packet_len, sizeof(uint32_t));
	memcpy(&(frame->packet_frame.packet_id), &packet_id, sizeof(uint32_t));

	if (write(cl->sockfd, (void*)(frame->u_data), packet_len_to_send) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}

	free(frame);

	return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
	FILE *fp;
	FILE *ch;
	void *recv_input;
	uint32_t packet_id;
	uint32_t file_name_size;
	uint32_t file_name_path;
	uint32_t packet_len;
	char *cmd_data;
	char *cmd_filename;
	char *file_path;
	char *file_name;
	char *file_name_split;
	char *result;
	char file_name_copy[50];
	char path[1024];
	union u_frame *pkg;
	union u_frame **packages;
	union u_frame *to_send;
	int i = 0;
	int j = 0;
	int size_to_receive;
	int sent_size;
	int offset = 0;
	int num_packages = 1;
	int one_package_size = 0;
	int nbytes;
	int size;

//	printf("Entered here socket : %d\n", cl->sockfd);

	recv_input = malloc(FRAME_LENGTH);
	if (!recv_input) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	if( (size = recv(cl->sockfd, recv_input, FRAME_LENGTH, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}

	packet_len = ntohl(*((uint32_t*)recv_input));
	packet_id = ntohl(*((uint32_t*)(recv_input + 1 * sizeof(uint32_t))));
	file_name_size = ntohl(*((uint32_t*)(recv_input + 2 * sizeof(uint32_t))));
	file_name_path = ntohl(*((uint32_t*)(recv_input + 3 * sizeof(uint32_t))));
	size_to_receive = packet_len + file_name_path + file_name_size + FRAME_LENGTH;

	printf("PACKET LEN : %d\n", packet_len);
	printf("PACKET ID IS : %d\n", packet_id);
	printf("FILE NAME SIZE : %d\n", file_name_size);
	printf("FILE NAME PATH SIZE : %d\n", file_name_path);

	if (packet_len < 0) {
		printf("Invalid packet len %d\n", packet_len);
		return MEMORY_ALLOCATION_ERROR;
	}

	num_packages = (packet_len / TCP_LIMIT) + 1;
	one_package_size = FRAME_LENGTH + file_name_size + file_name_path + packet_len / num_packages;
	printf("First package size is %d other is %d\n", one_package_size, (one_package_size - file_name_size - file_name_path));
	size_to_receive = packet_len;

	printf("There will be %d packages to receive\n", num_packages);

	packages = malloc(sizeof(union u_frame*) * (num_packages + 1));
	if (!packages) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	for (j = 0; j <= num_packages; j++) {
		packages[j] = malloc(one_package_size);
		if (!packages[j]) {
			printf("Memory corruption\n");
		}
		if (j != 0) {
			one_package_size = FRAME_LENGTH + (packet_len / num_packages);
		}
		if ( (size = recv(cl->sockfd, (packages[j])->u_data, one_package_size, 0)) >= 0) {
//			printf("RECEIVED package %d\n", j);
		} else {
//			printf("Receive DATA error\n");
		}
	}
	printf("j is %d\n", j);
/*
	pkg = malloc(size_to_receive);
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
*/

/*
	printf("--------------RECEIVED------------------\n");
	for (int k = 0; k < num_packages; k++) {
		printf("Package %d\n", k);
		for (int j = 0; j < one_package_size; j++) {
			printf("Byte %d hex %x char %c\n", j, (char*)((packages[k])->u_data)[j], (char*)((packages[k])->u_data)[j]);
		}
	}
	printf("--------------END RECEIVED------------------\n");
*/

	switch(packet_id) {

	case FILE_EXECUTE:

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


		memcpy(file_name, (packages[0]->u_data + FRAME_LENGTH), file_name_size);
		memcpy(file_path, (packages[0]->u_data + FRAME_LENGTH + file_name_size), file_name_path);

		file_name[file_name_size] = '\0';
		file_path[file_name_path] = '\0';

		strcpy(path, file_path);
		strcat(path, "/");
		strcat(path, file_name);

		printf("NEW PATH TO OPEN %s\n", path);
		printf("file_name ---->>%s<<---\n", file_name);
		printf("file_path ---->>%s<<---\n", file_path);

		fp = fopen(path, "wb+");
		if (!fp) {
			perror("fopen");
			sent_size = FRAME_LENGTH;
			to_send = malloc(sent_size);
			if (!to_send) {
				printf("Memory corruption\n");
				return MEMORY_ALLOCATION_ERROR;
			}
			to_send->packet_frame.packet_id = htonl(FILE_EXECUTE);
			to_send->packet_frame.packet_len = htonl(0);
			to_send->packet_frame.file_name_size = htonl(0);
			to_send->packet_frame.file_name_path_size = htonl(0);

			if (write(cl->sockfd, (void*)(to_send->u_data), sent_size) < 0) {
				printf("Error write");
				return ERR_WRITE;
			}

			printf("Reply sent\n");

			free(cmd_data);
			free(file_name);
			free(file_path);
			break;
		}

		cmd_data = malloc((packet_len / num_packages));
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}

		for (int i = 0; i <= num_packages; i++) {
			if (i == 0) {
				memcpy(cmd_data, (packages[i]->u_data + FRAME_LENGTH + file_name_size + file_name_path), (packet_len / num_packages));
			} else {
				memcpy(cmd_data, (packages[i]->u_data + FRAME_LENGTH), (packet_len / num_packages));
			}
			fwrite(cmd_data, (packet_len / num_packages), 1, fp);
		}

		fclose(fp);

		sent_size = FRAME_LENGTH + strlen(file_name) + strlen(file_path);

		to_send = malloc(sent_size);
		if (!to_send) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		to_send->packet_frame.packet_id = htonl(FILE_EXECUTE);
		to_send->packet_frame.packet_len = htonl(0);
		to_send->packet_frame.file_name_size = htonl(strlen(file_name));
		to_send->packet_frame.file_name_path_size = htonl(strlen(file_path));
		memcpy(to_send->packet_frame.cmd_data, file_name, strlen(file_name));
		memcpy(to_send->packet_frame.cmd_data + strlen(file_name), file_path, strlen(file_path));

		if (write(cl->sockfd, (void*)(to_send->u_data), sent_size) < 0) {
			printf("Error write");
			return ERR_WRITE;
		}

		free(to_send);
		free(file_name);
		free(file_path);
		free(cmd_data);

		break;

	case COMMAND_EXECUTE:
		cmd_data = malloc(packet_len);
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}

		memcpy(cmd_data, pkg->u_data + FRAME_LENGTH, packet_len);

		result = client_executor(cmd_data);

		sent_size = strlen(result) + FRAME_LENGTH;

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
		break;

	case FILE_UPLOAD:
		printf("GOT UPLOAD\n");

		cmd_filename = malloc(packet_len);
		if (!cmd_filename) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}

		memcpy(cmd_filename, (pkg->packet_frame.cmd_data), packet_len);

		cmd_filename[packet_len - 1] = '\0';
		printf("(%s)\n", cmd_filename);
		fp = fopen(cmd_filename, "rb");
		if (fp == NULL) {
			printf("File not exists\n");
			break;
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

//		printf("\nFILE SIZE %d\n", size);

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
		break;
	}

	free(recv_input);

	return SUCCESS;
}
