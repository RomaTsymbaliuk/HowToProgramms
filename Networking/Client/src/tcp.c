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
int tcp_client_send(struct client *cl)
{
	int n;

	n = 0;
	while (1) {
		char *buff = "CONNECT";
		if (write(cl->sockfd, buff, sizeof(buff)) < 0) {
			printf("%s\n", strerror(errno));
			printf("Write error\n");
			return ERR_WRITE;
		}
		sleep(1);
	}

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
		printf("SIZE : %d\n", size);
		packet_id = ntohl((((union u_frame*)recv_input)->pkt.header).packet_id);
		packet_len = ntohl((((union u_frame*)recv_input)->pkt.header).packet_len);
		cmd_size = ntohl((((union u_frame*)recv_input)->pkt.fields).cmd_len);
		printf("HERE3\n");
		printf("CMD_SIZE : %d\n", cmd_size);
		printf("PACKET_ID : %d\n", packet_id);
		printf("PACLET LEN : %d\n", packet_len);

	} else {
		printf("Receive error\n");
	}

	return SUCCESS;
}
