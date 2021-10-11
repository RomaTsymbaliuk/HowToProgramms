#ifndef TCP_H
#define TCP_H

#define MAX 80
#define TCP_ID 3
#define CMD_ID 100
#define PORT 8080
#define MIN_PORT 1024
#define MAX_PORT 65535
#define SA struct sockaddr
#define TCP_LIMIT 20

#include "type.h"
#include "errors.h"
#include "packet.h"

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

int tcp_server_write(struct menu *input);

int tcp_server_send_file(struct menu *input);

int tcp_server_upload(struct menu *input);


static struct server tcp_obj = {.sockfd = -1, .server_id = TCP, .cli_state = ">>>", .server_type = "TCP", .port = -1, .server_listen = tcp_server_listen,
                                .server_disconnect = tcp_server_disconnect,
								.server_bind = tcp_server_bind, .server_accept = tcp_server_accept, 
                                .server_read = tcp_server_read, .server_init = tcp_server_init, 
                                .server_write = tcp_server_write, .server_send = tcp_server_send_file,
                                .server_upload = tcp_server_upload};

extern struct server *server_object;

#endif /* __TCP_H__ */