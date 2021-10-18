#ifndef UDP_H
#define UDP_H

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#include "type.h"
#include "errors.h"
/**
 * Creates socket, waits for connections
 */
int udp_client_connect(struct client *cl, int port);

/**
 * Accepts connections, handshakes with clients
 */
int udp_client_send(struct client *cl, char *buff);

int udp_client_receive(struct client *cl);

static struct client udp_obj = {.sockfd = -1, .client_connect = udp_client_connect, .
                                client_id = UDP, .client_receive = udp_client_receive};


#endif /* __UDP_H__ */