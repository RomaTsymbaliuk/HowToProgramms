#ifndef SERVER_H
#define SERVER_H

#include "shell.h"
#include "tcp.h"
#include "errors.h"

struct server {
	int sockfd;
	char *server_type;
};

#endif /* __SERVER_H__ */