#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void shell_init(struct server *srv)
{
	fflush(stdin);
	server_object = srv;
	printf(SHELL_INIT);
}

void shell_loop()
{
	struct menu *input;
	int status;
	int i = -1;

	do {
		printf(">>>");
		input = shell_parse_input();
		if (input) {
			input->func();
			printf("\n");
			i++;
		}
	} while (i < MAX_SHELL_CMD );
}

int shell_exit()
{
	exit(SUCCESS);
}

int shell_help()
{
	printf(SHELL_HELP);
	return SUCCESS;
}

struct menu *shell_parse_input()
{
	struct menu *input;
	char str[MAX_CMD_LENGTH];
	char *pch;
	char **args = (char**)malloc(sizeof(char*) * MAX_ARGUMENTS_NUMBER);

	if (!args) {
		error(MEMORY_ALLOCATION_ERROR);
		return NULL;
	}
	memset(args, 0, MAX_ARGUMENTS_NUMBER * sizeof(char*));
	if (!input) {
		error(MEMORY_ALLOCATION_ERROR);
		return NULL;
	}
	if (!fgets(str, MAX_CMD_LENGTH, stdin)){
		error(PARSING_INPUT_ERROR);
		return NULL;
	}
	/*		Splitting line for commands and 		*/
	pch = strtok(str, " ");
	/*		Delete last enter in command if there are no 		*/
	if (pch[strlen(pch) - 1] == '\n') {
		pch[strlen(pch) - 1] = '\0';
	}

	/*		Too short command		*/
	if (strlen(pch) < 1) {
		return NULL;
	}
	/*		Not possible create switch that`s why ...		*/
	if (strcmp(pch, "exit") == 0) {
		input = &menus_objs[EXIT_ID];
	} else if (strcmp(pch, "help") == 0) {
		input = &menus_objs[HELP_ID];
	} else if (strcmp(pch, "connect") == 0 ) {
		input = &menus_objs[CONNECT_ID];
	} else if (strcmp(pch, "clear") == 0) {
		input = &menus_objs[CLEAR_ID];
	} else {
		printf("No such function!\n");
		return NULL;
	}
	int i = 0;
	while (pch != NULL ) {
		pch = strtok(NULL, " ");
		if (i < MAX_ARGUMENTS_NUMBER && pch && strcmp(pch, "\n") != 0) {
			args[i++] = strdup(pch);
		}
	}
	input->args = args;

	return input;
}

int shell_exec()
{
	return SUCCESS;
}

int shell_connect()
{
	printf("\nWorking on connect function...\n");
	server_object->server_listen(server_object);
	return SUCCESS;
}

int shell_clear()
{
	system("clear");
	return SUCCESS;
} 

//shell_connect - обвертка на server_connect void* 
// reconnect for client 