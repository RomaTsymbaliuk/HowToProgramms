#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef MENU_H
#define MENU_H
#include "menu.h"
#endif
#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif

int main(int argc, char *argv[]) {
	struct data *d;
	menu(argc, argv, d);
	return 1;
}
