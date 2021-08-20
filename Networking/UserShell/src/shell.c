#include <stdio.h>
#include "shell.h"
#include "errors.h"

int shell_init()
{
	printf(SHELL_INIT);
	return SUCCESS;
}

void shell_loop()
{
	printf("Shell loop started");
}

void shell_help()
{

}

int shell_parse_input()
{
	return SUCCESS;
}

int shell_exec_input()
{
	return SUCCESS;
}