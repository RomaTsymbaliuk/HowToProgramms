#ifndef TCP_H
#define TCP_H

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

struct tcp_server {
	int listen_fd;
	char *server_type;
};

static struct tcp_server tcp_obj = {-1, "TCP"};
/**
 * Creates socket, waits for connections
 */
int tcp_server_listen(struct tcp_server *server);

/**
 * Accepts connections, handshakes with clients
 */
int tcp_server_accept(struct tcp_server *server);

#endif /* __TCP_H__ */