#include <stdio.h>
#include "errors.h"

int user_error(int err)
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
		case ERR_CHOICE:
			printf("No such option!\n")
			return ERR_CHOICE;
		case ERR_BIND:
			printf("Bind socket error\n");
			return ERR_BIND;
		case ERR_LISTEN:
			printf("Socket listen error\n");
			return ERR_LISTEN;
		case ERR_CONNECT:
			printf("Socket connect error\n");
			return ERR_CONNECT;
		case ERR_TIMEOUT:
			printf("Time ended \n");
			return ERR_TIMEOUT;
		case ERR_SEND:
			printf("Send error occured\n");
			return ERR_SEND;
		case ERR_OPTION:
			printf("No such option\n");
			return ERR_OPTION;
		default:
			break;
	}
	return ERR_UNKNOWN;
}
