#ifndef TYPE_H
#define TYPE_H

enum {TCP = 1, UDP, NTP, DNS};

struct client {
	int sockfd;
	int client_id;
	void *client_type;
	int (*client_connect)(struct client *cl, int port);
	int (*client_send)(struct client *cl);
	int (*client_receive)(struct client *cl);
};

#endif