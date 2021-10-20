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

void shell_color()
{
	if ((*((int*)status_bar)) == SUCCESS) {
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
}

int shell_loop()
{
	int status = 0;
	int i = -1;
	//zapihnut v function color shell 
	do {
		shell_color();
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

	return SUCCESS;
}

int shell_help(struct menu *input)
{
	for (int i = 0; i < sizeof(helps)/sizeof(char*); i++){
		printf("%s\n", helps[i]);
	}

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
	printf("STATUS 0 %d \n", status);
	status = fn(args);
	printf("STATUS 1 %d \n", status);
	memcpy(status_bar, &status, sizeof(int));

	return SUCCESS;
}

int shell_menu_initializer(struct menu *menus_objs, int size)
{
	struct menu *shell_menu_objs;

	shell_menu_objs = malloc(sizeof(struct menu) * size);
	if (!shell_menu_objs) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	for (int i = 0; i < size; i++) {
		shell_menu_objs[i] = menus_objs[i];
	}
	shell_menu = shell_menu_objs;

	return SUCCESS;
}

int shell_parse_input(struct menu *menus_objs)
{
	struct menu *input;
	char str[MAX_CMD_LENGTH];
	char *pch;
	char *args[MAX_ARGUMENTS_NUMBER] = {NULL};
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
		if (shell_menu[i].cmd_name == NULL) {
			status = ERR_COMMAND;
			printf("No such command <%s> !\n", cmd_name);
			memcpy(status_bar, &status, sizeof(int));
			printf("%s", SHELL_HELP);
			free(cmd_name);
			return ERR_COMMAND;
		}
		// Check cmd? end print help
		if (!strncmp(cmd_name, shell_menu[i].cmd_name, strlen(shell_menu[i].cmd_name))) {
			input = &shell_menu[i];
			if (strchr(cmd_name, '?')) {
				printf("%s", input->help);
				free(cmd_name);
				return ERR_COMMAND;
			}
			break;
		}
		i++;
	}
	if (parse_args_num < input->min_args_size || parse_args_num > input->max_args_size) {
		printf("%s", input->help);
		printf("Arguments number doesn`t match ====>  [ parse : %d  <-->  required : [%d:%d] ]\n", parse_args_num, input->min_args_size, input->max_args_size);
		return ERR_COMMAND;
	}
	input->args = (void*)args;

	result = 0;
	if (input->func) {
		stack = CHECKALLOC(malloc(STACK_SIZE), "malloc");
		stack_top = stack + STACK_SIZE;
		child_pid = clone(shell_func_wrapper, stack_top, CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | CLONE_FILES | SIGCHLD, (void*)input);
		if (child_pid == -1) {
			free(stack);
			return ERR_FORK;
		}
		if (input->process_flags == FG) {
			for (int i = 0; i < 120; i++) {
				pid_t result_id = waitpid(child_pid, &status, WNOHANG);
				if (result_id == 0) {
					fprintf(stdout,"[%c]\n\r", WAIT_CHARS_CMD[i % WAIT_SYMBOLS]);
//					printf("\b\b");
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
				printf("\b\b\b");
			}
			printf("\n");
		} else if (input->process_flags == BG) {
				status = WAIT_CONDITION;
				memcpy(status_bar, &status, sizeof(int));
				return SUCCESS;
		}
	} else {
		printf("Field is empty\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	free(cmd_name);

	return SUCCESS;
}

int shell_clear(struct menu *input)
{
	// ne uzat sleep
	system("/usr/bin/clear");
	return SUCCESS;
} 

//shell_connect - обвертка на server_connect void* 
// reconnect for client 