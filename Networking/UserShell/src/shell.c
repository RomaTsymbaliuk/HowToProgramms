#include <stdio.h>
#include "shell.h"
#include "errors.h"

int shell_init()
{
	printf("Shell inited");
	return SUCCESS;
}
void shell_loop()
{
	printf("Shell loop started");
}
