struct stack{
        int top;
        int size;
        char **arr;
        struct linked_list *list;
};
struct linked_list {
	char *item;
	struct linked_list *next;
};
struct queue {
	int head;
	int tail;
	int size;
	char **arr;
	struct linked_list *list;
};

int stack_push(int flag_data_type, struct stack *st, char *str);
int stack_pop(int flag_data_type, struct stack *st);
int stack_is_empty(struct stack *st);
int stack_init(struct stack *st, int flag_data_type, int size);
int stack_is_full(struct stack *st);
int stack_print(int data_type, struct stack *st, int file_flag, char *filename);
int list_init(struct stack *st, struct queue *q);
int list_add(struct stack *st, char *item);
int list_remove(struct stack *st);
int queue_is_empty(struct queue *q);
int queue_is_full(struct queue *q);
int queue_pop(struct queue *q);
int queue_push(struct queue *q, char *str);
int queue_init(struct queue *q, int flag_data_type, int size);
void queue_show(struct queue *q);
