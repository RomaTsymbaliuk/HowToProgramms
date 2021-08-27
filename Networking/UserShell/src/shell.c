#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void shell_init()
{
	fflush(stdin);
	printf(SHELL_INIT);
}

int shell_loop()
{
	struct menu *input;
	int status;
	int i = -1;
	do {
		printf(">>>");
		input = shell_parse_input();
		if (input) {
			printf("\n***%s***\n", input->cmd_name);
			i++;
		} else {
			printf("Error! Try again\n");
		}
	} while (i < MAX_SHELL_CMD );
}

int shell_exit(struct menu *input)
{
	exit(SUCCESS);
}

int shell_help(struct menu *input)
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
		return NULL;
	}
	memset(args, 0, MAX_ARGUMENTS_NUMBER * sizeof(char*));
	if (!fgets(str, MAX_CMD_LENGTH, stdin)){
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
		printf("Too short command\n");
		return NULL;
	}
	char *cmd_name = strdup(pch);
	if (!cmd_name) {
		printf("Allocation problem\n");
		return NULL;
	}
	int i = 0;
	while (pch != NULL) {
		pch = strtok(NULL, " ");
		if (i < MAX_ARGUMENTS_NUMBER && pch && strcmp(pch, "\n") != 0) {
			args[i++] = strdup(pch);
		}
	}
	int parse_args_num = i;
	i = 0;
	while (1) {
		int status = strcmp(cmd_name, "END");
		if (!status) {
			break;
		}
		if (!strcmp(cmd_name, menus_objs[i].cmd_name)) {
			input = &menus_objs[i];
			break;
		}
		i++;
	}
	if (!strcmp(menus_objs[i].cmd_name, "END")) {
		printf("No such command <%s> !\n", cmd_name);
		return NULL;
	}
	if (input->args_size != parse_args_num) {
		printf("Arguments number doesn`t match ====>  [ parse : %d  <-->  required : %d\n ]", parse_args_num, input->args_size);
		return NULL;
	}
	input->args = (void*)args;
	int (*fn)(struct menu*);
	if (input->func) {
		fn = (int (*)(struct menu*))(input->func);
		int status = fn(input);
	} else {
		printf("Field is empty\n");
		return NULL;
	}
	return input;
}

int shell_exec(struct menu *input)
{
	return SUCCESS;
}

int shell_clear(struct menu *input)
{
	system("clear");
	return SUCCESS;
} 

//shell_connect - обвертка на server_connect void* 
// reconnect for client 