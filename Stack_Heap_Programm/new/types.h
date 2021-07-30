#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef STATIC_H
#define STATIC_H
#include "static.h"
#endif
#ifndef DYNAMIC_H
#define DYNAMIC_H
#include "dynamic.h"
#endif
#ifndef LISTS_H
#define LISTS_H
#include "lists.h"
#endif

int static_init(struct data *d);
int dynamic_init(struct data *d);
int list_init(struct data *d);
