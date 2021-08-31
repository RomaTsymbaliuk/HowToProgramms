#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

int flag_executed = 0;

void shell_init()
{
	fflush(stdin);
	printf(SHELL_INIT);
}

int shell_loop()
{
	struct menu *input;
	int status = 0;
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

void sig_handler(int signal)
{
	flag_executed = 1;
}

struct menu *shell_parse_input()
{
	struct menu *input;
	char str[MAX_CMD_LENGTH];
	char *pch;
	char **args = (char**)malloc(sizeof(char*) * MAX_ARGUMENTS_NUMBER);
	int status;
	int parse_args_num;
	int i = 0;
	int result;
	char *cmd_name;
	/*Child procces pid and execution status*/
	pid_t fork_status;
	pid_t child_pid;
	pid_t child_status;
	/* Print cmd while waiting for a client*/
	//92 ---> '\' in ASCII
	char WAIT_CHARS_CMD[WAIT_SYMBOLS] = {'/', '-', 92, '|'};
	/*Shared memory between child and parent processes*/
	void *shmem;
	int ret;


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
	cmd_name = strdup(pch);
	if (!cmd_name) {
		printf("Allocation problem\n");
		return NULL;
	}
	i = 0;
	while (pch != NULL) {
		pch = strtok(NULL, " ");
		if (i < MAX_ARGUMENTS_NUMBER && pch && strcmp(pch, "\n") != 0) {
			args[i++] = strdup(pch);
		}
	}
	parse_args_num = i;
	i = 0;
	while (1) {
		if (menus_objs[i].cmd_name == NULL) {
			printf("No such command <%s> !\n", cmd_name);
			printf("%s", SHELL_HELP);
			return NULL;
		}
		// Check cmd? end print help
		if (!strncmp(cmd_name, menus_objs[i].cmd_name, strlen(menus_objs[i].cmd_name))) {
			input = &menus_objs[i];
			if (strchr(cmd_name, '?')) {
				printf("%s", input->help);
				return NULL;
			}
			break;
		}
		i++;
	}
	if (input->args_size != parse_args_num) {
		printf("%s", input->help);
		printf("Arguments number doesn`t match ====>  [ parse : %d  <-->  required : %d ]\n", parse_args_num, input->args_size);
		return NULL;
	}
	input->args = (void*)args;
	int (*fn)(struct menu*);
	result = 0;
	if (input->func) {
		fn = (int (*)(struct menu*))(input->func);
		child_pid = fork();
		if (child_pid < 0) {
			return NULL;
		} else if (child_pid == 0) {
			fn(input);
			ret = kill(getppid(), SIGUSR1);
			return input;
		} else if (child_pid > 0){
			signal(SIGUSR1,sig_handler);
			for (int i = 0; i < 60; i++) {
				if (flag_executed) {
					flag_executed = 0;
					return input;
				}
				printf("[%c]\n", WAIT_CHARS_CMD[i % WAIT_SYMBOLS]);
				printf("MY PARENT : %d\n", getppid());
				sleep(1);
			}
			printf("Timeout ended!\n");
		}
	} else {
		printf("Field is empty\n");
		return NULL;
	}
	return NULL;
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