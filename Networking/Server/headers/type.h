#ifndef TYPE_H
#define TYPE_H

enum {TCP = 1, UDP, NTP, DNS};

struct server {
	int sockfd;
	int server_id;
	void *server_type;
	int (*server_listen)(struct server *sr);
	int (*server_bind)(struct server *sr);
	int (*server_accept)(struct server *sr);
	int (*server_read)(struct server *sr);
};

#endif