#pragma once

#include <string.h>

struct Stack {
	void* data; // stack items
	size_t cell_size; // size of each stack item
	size_t size; // current count of elements
	size_t capacity; // current possible size of stack
};

int StackCtor(Stack* stack, size_t cell_size);
int StackDtor(Stack* stack);

int StackPush (Stack * stack, void * item);
int StackTop  (Stack * stack, void * item);
int StackPop  (Stack * stack, void * item);
int StackPop  (Stack * stack);

int StackRealloc(Stack* stack, size_t new_capacity);

enum STACK_ERROR {
	STACK_OK = 0,
	STACK_MEM_ERROR = -1,
	STACK_NOT_CONSTRUCTED = -2,
	STACK_ITEM_ERROR = -3,
	STACK_EMPTY_ERROR = -4
};