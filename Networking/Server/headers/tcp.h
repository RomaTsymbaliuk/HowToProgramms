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
int tcp_server_listen();

int tcp_server_init();
/**
 * Accepts connections, handshakes with clients
 */
int tcp_server_accept();

int tcp_server_bind(int port);

int tcp_server_read();

int tcp_server_disconnect();

static struct server tcp_obj = {.sockfd = -1, .server_id = TCP, .cli_state = ">>>", .server_type = NULL, .port = -1, .server_listen = tcp_server_listen,
                                .server_disconnect = tcp_server_disconnect,
								.server_bind = tcp_server_bind, .server_accept = tcp_server_accept, 
                                .server_read = tcp_server_read, .server_init = tcp_server_init};


#endif /* __TCP_H__ */