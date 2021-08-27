#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
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
            printf("Write error\n");
            return ERR_WRITE;
        }
        sleep(1);
    }

    return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
    printf("TCP Client receive\n");
    return SUCCESS;
}
