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

int stack_push(int flag_data_type , char *str);
int stack_pop(int flag_data_type);
int stack_is_empty();
int stack_init( int flag_data_type, int size);
int stack_is_full();
int stack_print(int data_type, int file_flag, char *filename);
int list_init(struct stack *st, struct queue *q);
int list_add(struct stack *st, char *item);
int list_remove(struct stack *st);
int queue_is_empty();
int queue_is_full();
int queue_pop();
int queue_push(char *str);
int queue_init(int flag_data_type, int size);
void queue_print(int flag_data_type, int file_flag, char *filename);
