#ifndef MENU_H
#define MENU_H

#include "type.h"
#include "static_stack.h"

struct data *process_user_input(int argc, char *argv[]);
struct data *run_user_cmd(struct data *c);

extern struct data s_stack_obj;

#endif /* __MENU_H  */