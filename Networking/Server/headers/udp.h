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
int udp_server_listen(struct server *server);

/**
 * Accepts connections, handshakes with clients
 */
int udp_server_accept(struct server *server);

int udp_server_bind(struct server *server);

static struct server udp_obj = {.sockfd = -1, .server_listen = udp_server_listen,
								.server_bind = udp_server_bind, .server_accept = udp_server_accept};


#endif /* __UDP_H__ */