#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "tcp.h"

int tcp_client_connect(struct client *cl)
{
    short sockfd;
    int iRetval = -1;
    struct sockaddr_in remote = {0};
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return ERR_CREATE_SOCKET;
    }
    cl->sockfd = sockfd;

    remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);

    iRetval = connect(sockfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));

    if (iRetval == -1) {
        return ERR_CONNECT;
    }

    return SUCCESS;
}

/**
 * Accepts connections, handshakes with clients
 */
int tcp_client_send(struct client *cl, char *data)
{
    write(cl->sockfd, data, sizeof(data));
    return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
    printf("TCP Client receive\n");
    return SUCCESS;
}
