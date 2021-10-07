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
	uint32_t structures_size;
	uint32_t packet_len_to_send;

	frame = malloc(buff_len + 4 * sizeof(uint32_t));
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	cmd_size = buff_len;
	printf("WRITTEN CMD_SIZE %d\n", cmd_size);
//	structures_size = sizeof(struct packet_frame);

	packet_id = TCP;
	packet_len = cmd_size + 4 * sizeof(uint32_t);
	packet_len_to_send = packet_len;
// + reconnect for client + change to u_data

	printf("CMD SIZE : %d\n", cmd_size);
	printf("PACKET LEN TO SEND : %d\n", packet_len_to_send);

	packet_id = htonl(packet_id);
	packet_len = htonl(packet_len);

	memcpy(frame->packet_frame.cmd_data, buff, buff_len);
	memcpy(&(frame->packet_frame.packet_len), &packet_len, sizeof(uint32_t));

	printf("SENT FROM CLIENT : %d bytes\n", packet_len_to_send);

	if (write(cl->sockfd, (void*)(frame->u_data), packet_len_to_send) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}

	free(frame);

	return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
	void *recv_input;
	uint32_t packet_id;
	int size;
	char *cmd_data;
	char *result;
	void *recv_cmd;
	uint32_t packet_len;
	uint32_t cmd_size;
	int i = 0;

	printf("Entered here socket : %d\n", cl->sockfd);

	recv_input = malloc(8);

	if( (size = recv(cl->sockfd, recv_input, 8, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}
	packet_len = ntohl(*((uint32_t*)recv_input));
	packet_id = ntohl(*((uint32_t*)(recv_input + 1 * sizeof(uint32_t))));

	printf("PACKET LEN : %d\n", packet_len);
	printf("PACKET ID IS : %d\n", packet_id);

	if (packet_len < 0 || packet_len > 400) {
		printf("Invalid packet len\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	recv_cmd = malloc(2 * sizeof(uint32_t) + packet_len);
	if (!recv_cmd) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	if ( (size = recv(cl->sockfd, recv_cmd, packet_len + 2 * sizeof(uint32_t), 0)) >= 0) {
	} else {
		printf("Receive DATA error\n");
	}

	printf("--------------RECEIVED------------------\n");
	for (int k = 0; k < packet_len + 2 * sizeof(uint32_t); k++) {
		printf("Byte %d hex %x char %c\n", k, ((char*)recv_cmd)[k], ((char*)recv_cmd)[k]);
	}
	printf("--------------END RECEIVED------------------\n");

	if (packet_id == FILE_EXECUTE) {
		printf("SOME PLEASANT FILE TO EXECUTE\n");
	}
	else {
		printf("SOME PLEASANT COMMAND TO EXECUTE\n");
		cmd_data = malloc((packet_len));
		if (!cmd_data) {
			printf("Memory corruption\n");
			return MEMORY_ALLOCATION_ERROR;
		}

		memcpy(cmd_data, (recv_cmd + 2 * sizeof(uint32_t)), packet_len);
		printf("CMD_DATA : %s\n",cmd_data);
		result = client_executor(cmd_data);

		printf("RESULT SIZE : %d\n", strlen(result));

		tcp_client_send(cl, result, strlen(result));
	}

	free(recv_cmd);
	free(recv_input);

	return SUCCESS;

}
