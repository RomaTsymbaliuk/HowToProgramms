#define _GNU_SOURCE
#include <linux/sched.h>    /* Definition of struct clone_args */
#include <sched.h>          /* Definition of CLONE_* constants */
#include <sys/syscall.h>    /* Definition of SYS_* constants */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

#define STACK_SIZE (1024 * 1024) /* Stack size for cloned child */
#define ERREXIT(msg)                                                           \
  {                                                                            \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  }
#define CHECKALLOC(ptr, msg)                                                   \
  ({                                                                           \
    void *p = ptr;                                                             \
    if (p == NULL)                                                             \
      ERREXIT(msg);                                                            \
    p;                                                                         \
  })

// make in odna stroka

void shell_init()
{
	int status;

	fflush(stdin);
	printf(SHELL_INIT);

	status = WAIT_CONDITION;
	//check result do error exit
	status_bar = create_shared_memory(sizeof(int));
	memcpy(status_bar, &status, sizeof(int));
}

int shell_loop()
{
	int status = 0;
	int i = -1;
	//zapihnut v function color shell 
	do {
		if ( (*((int*)status_bar)) == SUCCESS) {
			printf("\033[0;32m");
			printf(">>> ");
			printf("\033[0m");
		}  else if ( (*((int*)status_bar)) == WAIT_CONDITION) {
			printf("\033[0;33m");
			printf(">>> ");
			printf("\033[0m");
		} else {
			printf("\033[0;31m");
			printf(">>> ");
			printf("\033[0m");
		}
		status = shell_parse_input();
		if (status == SUCCESS) {
			i++;
		} else {
			printf("\nTry again !\n");
		}
	} while (i < MAX_SHELL_CMD);
}

int shell_exit(struct menu *input)
{
	exit(SUCCESS);
}

int shell_help(struct menu *input)
{
	printf(SHELL_HELP); //vipilat`
	return SUCCESS;
}

int shell_func_wrapper(void *args)
{
	int (*fn)(struct menu*);
	args = (struct menu *) args;
	fn = (int (*)(struct menu*))(((struct menu*)args)->func);
	int status;

	status = WAIT_CONDITION;
	memcpy(status_bar, &status, sizeof(int));
	status = fn(args);
	memcpy(status_bar, &status, sizeof(int));

	return SUCCESS;
}

int shell_parse_input()
{
	struct menu *input;
	char str[MAX_CMD_LENGTH];
	char *pch;
	//Ne usat` malloc esli razmer static!!!
	char **args = (char**)malloc(sizeof(char*) * MAX_ARGUMENTS_NUMBER);
	int parse_args_num;
	int i = 0;
	int result;
	char *cmd_name;
	/*Child procces pid and execution status*/
	pid_t child_pid;
	/* Print cmd while waiting for a client*/
	//0x5c ---> '\' in ASCII
	char WAIT_CHARS_CMD[WAIT_SYMBOLS] = {'/', '-', 0x5c, '|'};
	char *stack;
	char *stack_top;
	int status;

	//Ne usat` malloc esli razmer static!!!
	stack = malloc(sizeof(STACK_SIZE));
	if (!stack) {
		return MEMORY_ALLOCATION_ERROR;
	}
	if (!args) {
		return MEMORY_ALLOCATION_ERROR;
	}
	memset(args, 0, MAX_ARGUMENTS_NUMBER * sizeof(char*));
	if (!fgets(str, MAX_CMD_LENGTH, stdin)){
		return ERR_READ;
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
		return ERR_COMMAND;
	}
	//memory leak
	cmd_name = strdup(pch);
	if (!cmd_name) {
		printf("Allocation problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	i = 0; //??? look def
	while (pch != NULL) {
		pch = strtok(NULL, " "); 
		 // *pch == '\n'
		if (i < MAX_ARGUMENTS_NUMBER && pch && strcmp(pch, "\n") != 0) {
			args[i++] = strdup(pch);
		}
	}
	parse_args_num = i;
	i = 0;
	while (1) {
		// ubrat zavisimost
		if (menus_objs[i].cmd_name == NULL) {
			status = ERR_COMMAND;
			printf("No such command <%s> !\n", cmd_name);
			memcpy(status_bar, &status, sizeof(int));
			printf("%s", SHELL_HELP);
			return ERR_COMMAND;
		}
		// Check cmd? end print help
		if (!strncmp(cmd_name, menus_objs[i].cmd_name, strlen(menus_objs[i].cmd_name))) {
			input = &menus_objs[i];
			if (strchr(cmd_name, '?')) {
				printf("%s", input->help);
				return ERR_COMMAND;
			}
			break;
		}
		i++;
	}
	if (input->args_size != parse_args_num) {
		printf("%s", input->help);
		printf("Arguments number doesn`t match ====>  [ parse : %d  <-->  required : %d ]\n", parse_args_num, input->args_size);
		return ERR_COMMAND;
	}
	input->args = (void*)args;

	result = 0;
	if (input->func) {
		stack = CHECKALLOC(malloc(STACK_SIZE), "malloc");
		stack_top = stack + STACK_SIZE;
		child_pid = clone(shell_func_wrapper, stack_top, CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | CLONE_FILES | SIGCHLD, (void*)input);
		if (child_pid == -1) {
			return ERR_FORK;
		}
		if (input->process_flags == FG) {
			for (int i = 0; i < 120; i++) {
				pid_t result_id = waitpid(child_pid, &status, WNOHANG);
				if (result_id == 0) {
					printf("%c\n", WAIT_CHARS_CMD[i % WAIT_SYMBOLS]);
				// Child still alive
				} else if (result_id == -1) {
				// Error 
					printf("ERROR EXECUTION\n");
					status = ERR_FORK;
					memcpy(status_bar, &status, sizeof(int));
					return ERR_FORK;
				} else {
				//Child process ended
					status = SUCCESS;
					memcpy(status_bar, &status, sizeof(int));
					return SUCCESS;
				}
				//sleep while waiting
				usleep(500 * 1000);
			}
		} else if (input->process_flags == BG) {
				status = WAIT_CONDITION;
				memcpy(status_bar, &status, sizeof(int));
				return SUCCESS;
		}
	} else {
		printf("Field is empty\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	return SUCCESS;
}

//rm
int shell_exec(struct menu *input)
{
	return SUCCESS;
}

int shell_clear(struct menu *input)
{
	// ne uzat sleep
	system("sleep 3 && ls -la && sleep 1 && clear");
	return SUCCESS;
} 

//shell_connect - обвертка на server_connect void* 
// reconnect for client 