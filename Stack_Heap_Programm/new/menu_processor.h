#ifndef __MENU_H
#define __MENU_H
#define MIN_SIZE 2
#define MAX_SIZE 100
#define MAX_STR_LEN 50
#define CMD_NUMBER 30
#include "static_stack.h"
#include "dynamic_stack.h"
#include "list_stack.h"

struct cmd {
	struct data *d;
	int (*fn)(struct data *d);
	void *get_data;
};

struct data *process_user_input(int argc, char *argv[]);
struct data *run_user_cmd(struct data *c);
extern struct data s_stack_obj;
extern struct data d_stack_obj;

#endif /* __MENU_H  */