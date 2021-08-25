#ifndef TCP_H
#define TCP_H

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#include "type.h"
#include "errors.h"

/**
 * Creates socket, waits for connections
 */
int tcp_server_listen(struct server *server);

/**
 * Accepts connections, handshakes with clients
 */
int tcp_server_accept(struct server *server);

int tcp_server_bind(struct server *server);

static struct server tcp_obj = {.sockfd = -1, .server_listen = tcp_server_listen,
								.server_bind = tcp_server_bind, .server_accept = tcp_server_accept};


#endif /* __TCP_H__ */