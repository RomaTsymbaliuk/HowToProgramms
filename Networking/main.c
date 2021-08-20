#include <stdio.h>
#include "errors.h"
#include "shell.h"

int main()
{
	shell_init();
	shell_loop();
	return SUCCESS;
}
