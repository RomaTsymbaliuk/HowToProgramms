#ifndef TYPE_H
#define TYPE_H

#include <arpa/inet.h>
#include <string.h>
#define INITIAL_SIZE 100

enum {TCP = 1, UDP, NTP, DNS};

struct buffer {
	void *data;
	size_t next;
	size_t size;
};

struct menu {
	char *cmd_name;
	char *help;
	void *func;
	void **args;
	int cmd_id;
	int args_size;
	int process_flags;
	struct buffer *buf;
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
	int (*server_write)(void *data);
};


struct buffer *new_buffer();
void reserve_space(struct buffer *b, size_t bytes);
void serialize_int(int x, struct buffer *b);
void serialize_menu(struct menu *input, struct buffer *buff);

struct server *server_object;

#endif