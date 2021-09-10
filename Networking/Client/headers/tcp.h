#ifndef TCP_H
#define TCP_H

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#include "type.h"
#include "errors.h"
#include "packet.h"

/**
 * Creates socket, waits for connections
 */
int tcp_client_connect(struct client *cl, int port);

/**
 * Accepts connections, handshakes with clients
 */
int tcp_client_send(struct client *cl);

int tcp_client_receive(struct client *cl);

static struct client tcp_obj = {.sockfd = -1, .client_connect = tcp_client_connect, .client_id = TCP,
								.client_send = tcp_client_send, .client_receive = tcp_client_receive};


#endif /* __TCP_H__ */