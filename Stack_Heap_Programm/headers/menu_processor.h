#ifndef __MENU_H
#define __MENU_H

#include "static_stack.h"
#include "dynamic_stack.h"
#include "list_stack.h"

#define MIN_STACK_SIZE 2
#define MAX_STACK_SIZE 100
#define MAX_STR_LEN 50
#define CMD_NUMBER 100

# define HELP_MENU      "NAME\n" \
                        "        main - create stack programm\n" \
                        "SYNOPSIS\n" \
                        "        ./main [OPTIONS]\n" \
                        "DESCRIPTION\n" \
                        "        \n" \
                        "        --create-static-stack           create stack with static array data structure, no size specified\n" \
                        "        --create-dynamic-stack [num]    create stack with dynamic array data structure, specify stack size\n" \
                        "        --create-list-stack             create stack with linked list data structure\n" \
                        "        --push                          add an element to the stack\n" \
                        "        --pop                           pop an element from the stack\n" \
                        "        --print                         print the stack\n" \
                        "LIMITS\n" \
                        "        DYNAMIC STACK  SIZE             100 elements\n" \
                        "        STRING LENGTH                   50 symbols\n" \
                        "        OPTIONS NUMBER                  30 options\n"

enum {PUSH, POP, PRINT, SLEEP};

struct cmd {
	void *user_data;
	int cmd_type;
	int size;
};

struct cmd_data {
	struct cmd **commands;
	struct data *d;
};

extern struct data s_stack_obj;
extern struct data d_stack_obj;

struct cmd_data *process_user_input(int argc, char *argv[]);
int run_user_cmd(struct cmd_data *c);

#endif /* __MENU_H  */