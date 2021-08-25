#include <stdio.h>
#include "Client.h"


int main()
{
	int choice;
	struct client *cli;
	printf("Choose protocol for data exchange\n");
	printf("1 ---- TCP\n2 ---- UDP\n3 ---- DNS\n4 ---- NTP\n");
	scanf("%d", &choice);

	switch(choice) {
		case TCP :
			cli = &tcp_obj;
			break;
		case UDP :
			cli = &udp_obj;
			break;
		case NTP :
			break;
		case DNS :
			break;
		default : 
			printf("No such protocol!\n");
			break;
	}
	if (cli->client_connect(cli) != SUCCESS) {
		printf("Connect failed \n");
		return ERR_CONNECT;
	}
	if (cli->client_send(cli, "hi man") != SUCCESS) {
		printf("Send error\n");
		return ERR_SEND;
	}
	return SUCCESS;
}