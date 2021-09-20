#include <stdio.h>
#include "udp.h"

int udp_client_connect(struct client *cl, int port)
{
    printf("UDP Client connect\n");
    return SUCCESS;
}

/**
 * Accepts connections, handshakes with clients
 */
int udp_client_send(struct client *cl, char *buff)
{
    printf("UDP Client send\n");
    return SUCCESS;
}

int udp_client_receive(struct client *cl)
{
    printf("UDP Client receive\n");
    return SUCCESS;
}
