#ifndef MENU_OBJS_H
#define MENU_OBJS_H

struct menu {
	char *cmd_name;
	char *help;
	void *func;
	void **args;
	int cmd_id;
	int min_args_size;
	int max_args_size;
	int process_flags;
};

#endif