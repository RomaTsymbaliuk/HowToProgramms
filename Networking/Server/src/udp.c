#include <stdio.h>
#include "udp.h"

int udp_server_listen()
{
	printf("UDP listen\n");
	return SUCCESS;
}

int udp_server_accept()
{
	printf("UDP accept\n");
	return SUCCESS;
}

int udp_server_bind(int port)
{
	printf("UDP bind\n");
	return SUCCESS;
}