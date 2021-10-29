#ifndef TYPE_H
#define TYPE_H

enum {TCP = 1, UDP, NTP, DNS};

struct client {
	int sockfd;
	int client_id;
	void *client_type;
	int (*client_connect)(struct client *cl, int port);
	int (*client_receive)(struct client *cl);
	char interface[20];
};

enum {FILE_EXECUTE = 1, COMMAND_EXECUTE, FILE_UPLOAD};

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