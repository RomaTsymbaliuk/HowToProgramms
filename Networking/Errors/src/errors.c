#include <stdio.h>
#include "errors.h"

int error(int err)
{
	switch(err)
	{
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error!\n");
			return MEMORY_ALLOCATION_ERROR;
		case FILE_NOT_EXISTS_ERROR:
			printf("File not exists!\n");
			return FILE_NOT_EXISTS_ERROR;
		case PARSING_INPUT_ERROR:
			printf("Input parsing error\n");
			return PARSING_INPUT_ERROR;
		case ERR_LISTEN:
			printf("Failed to listen on address\n");
			return ERR_LISTEN;
		case ERR_ACCEPT:
			printf("Failed accepting connection\n")
			return ERR_ACCEPT;
		case ERR_SOCKET_CREATION:
			printf("Failed to create socket!\n");
			return ERR_SOCKET_CREATION;
		case ERR_CHOICE:
			printf("No such option!\n")
			return ERR_CHOICE;
	}
	return ERR_UNKNOWN;
}
