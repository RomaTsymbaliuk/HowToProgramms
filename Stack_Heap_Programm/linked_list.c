#include "structures.h"
#include <stdlib.h>
#include <stdio.h>

int list_init(struct stack *st, struct queue *q) {
	
	struct linked_list *list = (struct linked_list *)malloc(sizeof(struct linked_list));
	if (!list) {
		printf("%s list allocation problem");
		return 0;
	}
	list->next = NULL;
	list->item = "NULL";
	if (st)
		st->list = list;
	if (q)
		q->list = list;
	
}
int list_add(struct stack *st, char *item) {
	
	struct linked_list *tmp = st->list;
	while ( tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = (struct linked_list *)malloc(sizeof(struct linked_list));
	(tmp->next)->item = item;
	(tmp->next)->next = NULL;
	
	printf("Happy list add function !\n");
}
int list_remove(struct stack *st) {
	
	struct linked_list *tmp = st->list;

	while ( ((tmp->next)->next) != NULL){
		tmp = tmp->next;

	}
	struct linked_list *smp = tmp->next;
	tmp->next = NULL;
	smp->item = NULL;
	free(smp);	
}
