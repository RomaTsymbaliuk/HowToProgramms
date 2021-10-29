#ifndef TCP_H
#define TCP_H

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define TCP_LIMIT 2
#define MAX_COMMAND_LEN 3096

#include "type.h"
#include "errors.h"
#include "packet.h"

/**
 * Creates socket, waits for connections
 */
int tcp_client_connect(struct client *cl, int port);

int tcp_client_receive(struct client *cl);

int tcp_client_send_file_handler(union u_data_frame **packages, union u_data_frame *last_pkg, int file_name_size, 
                                 int file_name_path, int last_pkg_size, int num_packages, struct client *cl);

int tcp_client_execute_handler(union u_data_frame **packages, union u_data_frame *last_pkg, 
                                int last_pkg_size, int num_packages, struct client *cl);

int tcp_client_upload_file_handler(union u_data_frame **packages, int file_name_size,
                                   int file_name_path, int num_packages, struct client *cl);


static struct client tcp_obj = {.sockfd = -1, .client_connect = tcp_client_connect, .client_id = TCP,
								.client_receive = tcp_client_receive};


#endif /* __TCP_H__ */