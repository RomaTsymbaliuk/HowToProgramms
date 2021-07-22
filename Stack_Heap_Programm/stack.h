struct stack{
	int top;
	int size;
	char **arr;
};
int stack_push(char *str);
int stack_pop();
int stack_is_empty(struct stack *st);
int stack_init(struct stack *st, int flag_data_type, int size);
int stack_is_full(struct stack *st);
