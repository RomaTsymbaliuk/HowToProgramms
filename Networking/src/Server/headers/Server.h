#ifndef SERVER_H
#define SERVER_H

#include "shell.h"
#include "menu_objs.h"
#include "tcp.h"
#include "shared.h"
#include "packet.h"
#include "udp.h"

#define SERVER_HELP     "NAME\n" \
						"        main - server - client reverse shell programm\n" \
						"SYNOPSIS\n" \
						"        ./server [OPTIONS] \n" \
						"DESCRIPTION\n" \
						"        \n" \
						"It is a server help. Enter specific options for different server connections. Options are written below\n" \
						"\n" \
						"			--tcp				------>								create server with tcp connection\n" \
						"			--udp				------>								create server with udp connection\n" \
						"			--ntp				------>								create server with ntp connection\n" \
						"			--dns				------>								create server with dns connection\n" \
						"			--help				------>								show this help\n" \
						"\n" \
						"EXAMPLE\n" \
						"        ./server --tcp\n" \
						"\n" \
						"\n"

int server_exploit(struct menu *input);
int server_connect(struct menu *input);
int server_disconnect(struct menu *input);
int server_register(int type);
int server_read();

struct server *server_object;

#endif /* __SERVER_H__ */