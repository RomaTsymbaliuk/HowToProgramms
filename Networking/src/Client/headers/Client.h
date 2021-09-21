#ifndef TCP_CLIENT
#define TCP_CLIENT

#define MAX 80
#define MIN_PORT 1024
#define MAX_PORT 65535
#define PORT 8080
#define SA struct sockaddr

#define HELP 			"NAME\n" \
						"        	main - client - client reverse shell programm\n" \
						"SYNOPSIS\n" \
						"        	./main [OPTIONS]\n" \
						"DESCRIPTION\n" \
						"\n" \
						"It is a client help. Enter command and arguments for the command. Special commands are written below\n" \
						"\n" \
						"		--tcp [PORT]				------>								create client with tcp connection\n" \
						"		--udp [PORT]				------>								create client with udp connection\n" \
						"		--ntp [PORT]				------>								create client with ntp connection\n" \
						"		--dns [PORT]				------>								create client with dns connection\n" \
						"		--help [PORT]				------>								show this help\n" \
						"\n" \
						"EXAMPLE\n" \
						"		./main --tcp\n" \
						"\n" \
						"\n" \
						"LIMITS\n\n" \
						"		PORT: 1024 --- 65535\n" \
						"SPECIAL COMMANDS\n\n" \
						"         	exit     ------    to exit the shell\n" \
						"         	cmd?     ------    to show this help\n" \

#include "errors.h"
#include "tcp.h"
#include "udp.h"

#endif /* TCP_CLIENT */