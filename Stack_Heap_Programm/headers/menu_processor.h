#ifndef __MENU_H
#define __MENU_H

#include "static_stack.h"
#include "dynamic_stack.h"
#include "list_stack.h"

#define MIN_SIZE 2
#define MAX_SIZE 100
#define MAX_STR_LEN 50
#define CMD_NUMBER 30

enum {PUSH, POP, PRINT};

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