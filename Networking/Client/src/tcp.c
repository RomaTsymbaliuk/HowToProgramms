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
	uint32_t cmd_id = 5;
	uint32_t packet_len = 20;
	uint32_t cmd_len = 56;
	char buffer[1024];

	for (int i = 0; i < 1024; i++) {
		buffer[i] = buff[i];
	}
/*
	printf("_________BUFF__________\n");
	printf("%s\n", buff);
	printf("_______________________\n");

	printf("________BUFFER_________\n");
	printf("%s\n", buffer);
	printf("________________________\n");
*/
	frame = malloc(sizeof(union u_frame));
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	cmd_id = htonl(cmd_id);
	packet_len = htonl(packet_len);
	cmd_len = htonl(cmd_len);

	printf("SIZE OF BUFFER : %d\n", sizeof(buffer));
	memcpy(&(frame->pkt.fields.cmd_data), buffer, sizeof(buffer));
	memcpy(&(frame->pkt.fields.cmd_id), &cmd_id, sizeof(cmd_id));
	memcpy(&(frame->pkt.fields.cmd_len), &cmd_len, sizeof(cmd_len));

	if (write(cl->sockfd, frame, sizeof(*frame)) < 0) {
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
	uint32_t packet_len;
	uint32_t cmd_size;
	char *cmd_data;
	char *result;
	struct packet_frame *frame;

	recv_input = malloc(sizeof(struct packet_frame));
	if (!recv_input) {
		return MEMORY_ALLOCATION_ERROR;
	}

	printf("Entered here socket : %d\n", cl->sockfd);
	printf("RECEIVED STRUCTURE:\n");

	//cast to struct
	//payload after struct header -> (alligned)
	if( (size = recv(cl->sockfd, recv_input, sizeof(union u_frame), 0)) >= 0) {
		packet_id = ntohl((((union u_frame*)recv_input)->pkt.header).packet_id);
		packet_len = ntohl((((union u_frame*)recv_input)->pkt.header).packet_len);
		cmd_size = ntohl((((union u_frame*)recv_input)->pkt.fields).cmd_len);
		cmd_data = ((((union u_frame*)recv_input)->pkt.fields).cmd_data);
		printf("HERE3\n");
		printf("CMD_SIZE : %d\n", cmd_size);
		printf("PACKET_ID : %d\n", packet_id);
		printf("PACKET LEN : %d\n", packet_len);
		printf("DATA : %s\n", cmd_data);
		result = client_executor(cmd_data);
		tcp_client_send(cl, result);
	} else {
		printf("Receive error\n");
	}

	return SUCCESS;
}
