#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "errors.h"

int shell_init()
{
	printf(SHELL_INIT);
	return SUCCESS;
}

void shell_loop()
{
	struct cmd *inputs[MAX_SHELL_CMD] = {NULL};
	int status;
	int i = -1;

	do {
		printf(">>>");
		inputs[++i] = shell_parse_input();
		printf("\n");
	} while (i < MAX_SHELL_CMD && inputs[i] && (strcmp(inputs[i]->command, "exit") != 0 ));
}

void shell_help()
{

}

struct cmd *shell_parse_input()
{
	struct cmd *input = (struct cmd*)malloc(sizeof(struct cmd));
	char str[MAX_CMD_LENGTH];
	char *pch;
	input->args = (char**)malloc(sizeof(char*) * MAX_ARGUMENTS_NUMBER);

	if (!input->args) {
		error(MEMORY_ALLOCATION_ERROR);
		return NULL;
	}
	memset(input->args, 0, MAX_ARGUMENTS_NUMBER * sizeof(char*));
	if (!input) {
		error(MEMORY_ALLOCATION_ERROR);
		return NULL;
	}
	if (!fgets(str, MAX_CMD_LENGTH, stdin)){
		error(PARSING_INPUT_ERROR);
		return NULL;
	}
	/*  Splitting line for commands and arguments  */
	pch = strtok(str, " ");
	/*  Delete last enter in command if there are no arguments  */
	if (pch[strlen(pch) - 1] == '\n') {
		pch[strlen(pch) - 1] = '\0';
	}
	input->command = strdup(pch);
	int i = 0;
	while (pch != NULL ) {
		pch = strtok(NULL, " ");
		if (i < MAX_ARGUMENTS_NUMBER && pch && strcmp(pch, "\n") != 0) {
			input->args[i++] = strdup(pch);
		}
	}
	return input;
}

int shell_exec_input()
{
	return SUCCESS;
}