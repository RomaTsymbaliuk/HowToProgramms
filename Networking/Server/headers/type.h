#ifndef TYPE_H
#define TYPE_H

enum {TCP = 1, UDP, NTP, DNS};

struct server {
	int sockfd;
	int server_id;
	char cli_state[20];
	void *server_type;
	int port;
	int (*server_listen)();
	int (*server_bind)(int port);
	int (*server_accept)();
	int (*server_read)();
	int (*server_disconnect)();
	int (*server_init)();
};

static struct server *server_object;

#endif