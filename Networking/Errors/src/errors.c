#include <stdio.h>
#include "errors.h"

void choose_error(int err)
{
	switch(err)
	{
		case MEMORY_ALLOCATION_ERROR:
			printf("Memory allocation error!\n");
			break;
		case FILE_NOT_EXISTS_ERROR:
			printf("File not exists!\n");
			break;	
	}
}
