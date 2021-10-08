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
//	printf("WRITTEN CMD_SIZE %d\n", cmd_size);
//	structures_size = sizeof(struct packet_frame);

	packet_len = cmd_size + FRAME_LENGTH;
	packet_len_to_send = packet_len;
// + reconnect for client + change to u_data

//	printf("CMD SIZE : %d\n", cmd_size);
//	printf("PACKET LEN TO SEND : %d\n", packet_len_to_send);

	packet_id = htonl(COMMAND_EXECUTE);
	packet_len = htonl(packet_len);

	memcpy(frame->packet_frame.cmd_data, buff, buff_len);
	memcpy(&(frame->packet_frame.packet_len), &packet_len, sizeof(uint32_t));
	memcpy(&(frame->packet_frame.packet_id), &packet_id, sizeof(uint32_t));

//	printf("SENT FROM CLIENT : %d bytes\n", packet_len_to_send);

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
	int nbytes;
	void *recv_input;
	uint32_t packet_id;
	uint32_t file_name_size;
	uint32_t file_name_path;
	int size;
	char *cmd_data;
	char *cmd_filename;
	char *file_path;
	char *file_name;
	char *result;
	void *recv_cmd;
	union u_frame *pkg;
	union u_frame *to_send;
	uint32_t packet_len;
	uint32_t cmd_size;
	char cmd[50] = {0};
	char path[50] = "";
	char check[50] = "file ";
	int i = 0;
	int is_text_file = 0;

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

	printf("PACKET LEN : %d\n", packet_len);
	printf("PACKET ID IS : %d\n", packet_id);
	printf("FILE NAME SIZE : %d\n", file_name_size);
	printf("FILE NAME PATH SIZE : %d\n", file_name_path);
	pkg = malloc(packet_len + FRAME_LENGTH);
	if (!pkg) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	if (packet_len < 0 || packet_len > 1000000) {
		printf("Invalid packet len %d\n", packet_len);
		return MEMORY_ALLOCATION_ERROR;
	}

	if ( (size = recv(cl->sockfd, pkg->u_data, packet_len + FRAME_LENGTH, 0)) >= 0) {
	} else {
		printf("Receive DATA error\n");
	}

/*
	printf("--------------RECEIVED------------------\n");
	for (int k = 0; k < packet_len + 2 * sizeof(uint32_t); k++) {
		printf("Byte %d hex %x char %c\n", k, ((char*)recv_cmd)[k], ((char*)recv_cmd)[k]);
	}
	printf("--------------END RECEIVED------------------\n");
*/

	if (packet_id == FILE_EXECUTE) {

		cmd_data = malloc(packet_len);
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
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

		memcpy(file_name, pkg->packet_frame.cmd_data, file_name_size);
		memcpy(file_path, (pkg->packet_frame.cmd_data + file_name_size), file_name_path);
		memcpy(cmd_data, (pkg->packet_frame.cmd_data + file_name_size + file_name_path), packet_len);

		strcat(path, file_path);
		strcat(path, "/");
		strcat(path, file_name);

		strcat(check, path);

		fp = fopen(path, "wb");
		if (!fp) {
			perror("fopen");
			return ERR_READ;
		}

		fwrite(cmd_data , packet_len, 1, fp);

		fclose(fp);

		strcat(cmd, "chmod a+x ");
		strcat(cmd, path);
		strcat(cmd, " && cd ");
		strcat(cmd, file_path);
		strcat(cmd, " && ");
		strcat(cmd, "./");
		strcat(cmd, file_name);

		result = client_executor(check);

		result[strlen(result) - 1] = '\0';
		printf("RESSSUUULT %s\n", result);
		char *token = strtok(result, " ");

		while( token != NULL ) {
			if (strcmp(token, "data") == 0) {
				printf("It is a text file. Only save\n");
				is_text_file = 1;
			}
			printf("token %s\n", token);
			token = strtok(NULL, " ");
		}
		if (!is_text_file) {
			result = client_executor(cmd);
		}
		to_send = malloc(strlen(result) + FRAME_LENGTH + strlen(file_name) + strlen(file_path));
		if (!to_send) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
		to_send->packet_frame.packet_id = htonl(0);
		to_send->packet_frame.packet_len = htonl(strlen(result));
		to_send->packet_frame.file_name_size = htonl(strlen(file_name));
		to_send->packet_frame.file_name_path_size = htonl(strlen(file_path));
		memcpy(to_send->packet_frame.cmd_data, file_name, strlen(file_name));
		memcpy(to_send->packet_frame.cmd_data + strlen(file_name), file_path, strlen(file_path));
		memcpy(to_send->packet_frame.cmd_data + strlen(file_name) + strlen(file_path), result, strlen(result));

		if (write(cl->sockfd, (void*)(to_send->u_data), (strlen(file_name) + strlen(file_path) + strlen(result) + FRAME_LENGTH)) < 0) {
			printf("Error write");
			return ERR_WRITE;
		}


		free(to_send);

	} else if (packet_id == COMMAND_EXECUTE){
//		printf("SOME PLEASANT COMMAND TO EXECUTE\n");
		cmd_data = malloc(packet_len);
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}

		memcpy(cmd_data, pkg->packet_frame.cmd_data, packet_len);
		printf("CMD_DATA : %s\n",cmd_data);
		result = client_executor(cmd_data);

		to_send = malloc(strlen(result) + FRAME_LENGTH);
		if (!to_send) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}

		memcpy(to_send->packet_frame.cmd_data, result, strlen(result));

		to_send->packet_frame.packet_id = htonl(0);
		to_send->packet_frame.packet_len = htonl(strlen(result));
		to_send->packet_frame.file_name_size = htonl(0);
		to_send->packet_frame.file_name_path_size = htonl(0);

		if (write(cl->sockfd, (void*)(to_send->u_data), (FRAME_LENGTH + strlen(result))) < 0) {
			printf("Error write");
			return ERR_WRITE;
		}

		free(to_send);

	} else if (packet_id == FILE_UPLOAD) {

		cmd_filename = malloc(packet_len);
		if (!cmd_filename) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}
//		printf("HERE PACKET LEN : %d\n", packet_len);
		memcpy(cmd_filename, (pkg->packet_frame.cmd_data), packet_len);
//		printf("SEARCHING FOR FILE %s\n", cmd_filename);
		fp = fopen(cmd_filename, "rb");
		if (fp == NULL) {
			printf("File not exists\n");
			return ERR_READ;
		}

		if (fp) {

			fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			printf("\nFILE SIZE %d\n", size);

			cmd_data = malloc(size);
			if (!cmd_data) {
				printf("Memory corruption\n");
				return MEMORY_ALLOCATION_ERROR;
			}

			fread(cmd_data, size, 1, fp);
			tcp_client_send(cl, cmd_data, size);

			free(cmd_data);
			free(cmd_filename);
			fclose(fp);
		}
	}

	free(recv_input);

	return SUCCESS;

}
