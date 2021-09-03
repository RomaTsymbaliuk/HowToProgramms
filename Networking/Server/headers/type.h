#ifndef TYPE_H
#define TYPE_H

enum {TCP = 1, UDP, NTP, DNS};

struct menu {
	char *cmd_name;
	char *help;
	void *func;
	void **args;
	int cmd_id;
	int args_size;
	int process_flags;
};

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
	int (*server_write)(struct menu *);
};

struct server *server_object;

#endif