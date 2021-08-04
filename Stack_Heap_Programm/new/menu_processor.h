#ifndef __MENU_H
#define __MENU_H
#define MIN_SIZE 2
#define MAX_SIZE 100
#define MAX_STR_LEN 50
#include "static_stack.h"
#include "string.h"

struct data *process_user_input(int argc, char *argv[]);
struct data *run_user_cmd(struct data *c);
extern struct data s_stack_obj;

#endif /* __MENU_H  */