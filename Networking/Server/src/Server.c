#include <stdio.h>
#include "Server.h"

int main()
{
	int choice;
	struct server *srv = NULL;

	printf("Choose protocol for data exchange\n");
	printf("1 ---- TCP\n2 ---- UDP\n3 ---- DNS\n4 ---- NTP\n");
	scanf("%d", &choice);

	switch(choice) {
		case TCP :
			srv = &tcp_obj;
			break;
		case UDP :
			srv = &udp_obj;
			break;
		case NTP :
			break;
		case DNS :
			break;
		default : 
			printf("No such protocol!\n");
			break;
	}

	shell_init(srv);
	shell_loop();
}