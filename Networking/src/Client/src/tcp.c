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
int tcp_client_send(struct client *cl, char *buff)
{
	union u_frame *frame;
	uint32_t cmd_id;
	uint32_t packet_len;
	uint32_t cmd_len;
	uint32_t cmd_size;
	uint32_t structures_size;
	uint32_t packet_len_to_send;

	frame = malloc(sizeof(union u_frame));
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	cmd_size = strlen(buff);
//	structures_size = sizeof(struct packet_frame);

	cmd_id = TCP;
	packet_len = cmd_size + 4 * sizeof(uint32_t);
	packet_len_to_send = packet_len;
// + reconnect for client + change to u_data

	printf("CMD SIZE : %d\n", cmd_size);
	printf("PACKET LEN TO SEND : %d\n", packet_len_to_send);


	cmd_id = htonl(cmd_id);
	packet_len = htonl(packet_len);
	cmd_size = htonl(cmd_size);


	memcpy(frame->packet_frame.cmd_data, buff, strlen(buff) * sizeof(char));
	memcpy(&(frame->packet_frame.cmd_id), &cmd_id, sizeof(cmd_id));
	memcpy(&(frame->packet_frame.cmd_len), &cmd_size, sizeof(cmd_size));
	memcpy(&(frame->packet_frame.packet_len), &packet_len, sizeof(uint32_t));


	printf("SENT FROM CLIENT : %d bytes\n", packet_len_to_send);

	if (write(cl->sockfd, (void*)(frame->u_data), packet_len_to_send) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}

	printf("\n\n\nSent answer");

	return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
	void *recv_input;
	uint32_t packet_id;
	uint32_t comd_id;
	int size;
	uint32_t cmd_size;
	char *cmd_data;
	char *result;
	union u_frame frame;
	uint32_t packet_len;
	int i = 0;

	printf("Entered here socket : %d\n", cl->sockfd);

	if( (size = recv(cl->sockfd, frame.u_data, 4, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}
	packet_len = ntohl(*((uint32_t*)frame.u_data));
	printf("Need to read %d bytes\n", packet_len);
	result = (char*)malloc(sizeof(char) * (packet_len + 1));

	if ( (size = recv(cl->sockfd, frame.u_data, packet_len + 15, 0)) >= 0) {
	} else {
		printf("Receive DATA error\n");
	}

	cmd_data = malloc((packet_len + 1));
	if (!cmd_data) {
		printf("Memory corruption\n");
		return MEMORY_ALLOCATION_ERROR;
	}


	for (int k = 15; k < packet_len + 15; k++) {
//		printf("BYTE %d : hex %x char : %c\n", k, frame.u_data[k], frame.u_data[k]);
		cmd_data[k - 15] = frame.u_data[k];
	}


	cmd_data = cmd_data + 1;


	result = client_executor(cmd_data);

	printf("RESULT SIZE : %d\n", strlen(result));

	tcp_client_send(cl, result);

	return SUCCESS;
}
