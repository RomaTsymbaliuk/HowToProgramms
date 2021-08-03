#ifndef MENU_H
#define MENU_H

#include "data.h"
#include "type.h"
#include "static_stack.h"

struct obj *process_user_input(int argc, char *argv[]);
struct data *run_user_cmd(struct obj *c);
extern struct obj s_stack_obj;

#endif /* __MENU_H  */