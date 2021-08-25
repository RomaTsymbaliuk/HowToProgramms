#include <stdio.h>
#include "Server.h"

int main()
{
	int choice;
	void *server = NULL;

	printf("Choose protocol for data exchange\n");
	printf("1 ---- TCP\n2 ---- UDP\n3 ---- DNS\n4 ---- NTP\n");
	scanf("%d", &choice);

	switch(choice) {
		case 1 :
			server = &tcp_obj;
			break;
		case 2 :
			break;
		case 3 :
			break;
		case 4 :
			break;
		default : 
			printf("No such protocol!\n");
			break;
	}

	shell_init(server);
	shell_loop();
}