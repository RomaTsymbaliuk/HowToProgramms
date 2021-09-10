#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <errno.h>
#include "tcp.h"

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
	int packet_id;
	uint32_t comd_id;
	int size;
	int packet_len;

	recv_input = malloc(sizeof(struct packet_frame));

	printf("Entered here socket : %d\n", cl->sockfd);

	//cast to struct
	//payload after struct header -> (alligned)
	if( (size = recv ( cl->sockfd,  recv_input, sizeof(recv_input), 0)) >= 0) {
		packet_id = ntohl((((struct packet_frame*)recv_input)->header).packet_id);
		comd_id = ntohl(((struct packet_frame*)recv_input)->cmd_id);
		printf("NETWORK : %d\n", comd_id);
		packet_len = ntohl((((struct packet_frame*)recv_input)->header).packet_len);
		printf("HEADER_ID : %d  CMD_ID : %d LENGTH : %d\n", packet_id, comd_id, packet_len);
	} else {
		printf("Receive error\n");
	}

	return SUCCESS;
}
