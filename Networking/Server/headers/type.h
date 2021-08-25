#ifndef TYPE_H
#define TYPE_H

struct server {
	int sockfd;
	void *server_type;
	int (*server_listen)(struct server *sr);
	int (*server_bind)(struct server *sr);
	int (*server_accept)(struct server *sr);
};

#endif