#ifndef TYPE_H
#define TYPE_H

enum {TCP = 1, UDP, NTP, DNS};

struct client {
	int sockfd;
	int client_id;
	void *client_type;
	int (*client_connect)(struct client *cl, int port);
	int (*client_send)(struct client *cl, char *buff);
	int (*client_receive)(struct client *cl);

};

struct menu {
	char *cmd_name;
	char *help;
	void *func;
	void **args;
	int cmd_id;
	int args_size;
	int process_flags;
};


#endif