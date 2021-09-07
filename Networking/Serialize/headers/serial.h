#ifndef SERIALIZE_H
#define SERIALIZE_H

#define INITIAL_SIZE 100

#include "menu_objs.h"
#include "packet.h"

void reserve_space(struct packet *b, size_t bytes);
void serialize_int(int x, struct packet *b);
void serialize_menu(struct menu *input, struct packet *b);

#endif