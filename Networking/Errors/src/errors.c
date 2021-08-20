#include <stdio.h>
#include "errors.h"

int error(int err)
{
	switch(err)
	{
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error!\n");
			return MEMORY_ALLOCATION_ERROR;
			break;
		case FILE_NOT_EXISTS_ERROR:
			printf("File not exists!\n");
			return FILE_NOT_EXISTS_ERROR;
			break;
		case PARSING_INPUT_ERROR:
			printf("Input parsing error\n");
			return PARSING_INPUT_ERROR;
			break;
	}
}
