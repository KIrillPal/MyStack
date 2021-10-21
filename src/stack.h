#ifndef STACK_H
#define STACK_H

#include "stack_errors.h"


#define PROTECTECTION_LEVEL 1

#if (PROTECTECTION_LEVEL >= 1)
#define STACK_USE_CANARY
#endif

#if (PROTECTECTION_LEVEL >= 2)
#define STACK_USE_HASH
#endif

#define CANARY_VALUE 0xf1e2d3c4
#define CANARY_SIZE  4


#define IS_INITALIZED  0x12345678
#define NOT_INITALIZED 0xDeadDead

struct Stack 
{

#ifdef STACK_USE_CANARY
	int left_canary = NOT_INITALIZED;
#endif

	int init_status  = NOT_INITALIZED; // flag shows if stack is initalized
	size_t cell_size = NOT_INITALIZED; // size of each stack item in bytes
	size_t size      = NOT_INITALIZED; // current count of elements
	size_t capacity  = NOT_INITALIZED; // current possible size of stack
	void* data       = (void*)NOT_INITALIZED; // stack items

#ifdef STACK_USE_CANARY
	int right_canary = NOT_INITALIZED;
#endif

};

StackResult StackCtor (Stack* stack, size_t cell_size);
StackResult StackDtor (Stack* stack);

StackResult StackPush (Stack * stack, void * item);
StackResult StackTop  (Stack * stack, void * item);
StackResult StackPop  (Stack * stack, void * item);
StackResult StackPop  (Stack * stack);

StackResult StackRealloc(Stack* stack, size_t new_capacity);

#endif