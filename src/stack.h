#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#include "stack_errors.h"


#define PROTECTECTION_LEVEL 2

#if (PROTECTECTION_LEVEL >= 1)
#define STACK_USE_CANARY
#endif

#if (PROTECTECTION_LEVEL >= 2)

#define STACK_USE_HASH
#define STACK_USE_SELF_HASH

#endif

#if (PROTECTECTION_LEVEL >= 3)
#define STACK_USE_DATA_HASH
#endif

#define CANARY_VALUE   0xf1e2d3c4
#define CANARY_SIZE    4
#define HASH_REF_VALUE 0xC0


#define IS_INITALIZED  0xC0BAB0BA
#define NOT_INITALIZED 0xDeadDead

struct Stack 
{

#ifdef STACK_USE_CANARY
	int left_canary = NOT_INITALIZED;
#endif

#ifdef STACK_USE_SELF_HASH
	int  left_hash_ref = NOT_INITALIZED;
#endif

	int init_status  = NOT_INITALIZED; // flag shows if stack is initalized
	size_t cell_size = NOT_INITALIZED; // size of each stack item in bytes
	size_t size      = NOT_INITALIZED; // current count of elements
	size_t capacity  = NOT_INITALIZED; // current possible size of stack
	void* data       = (void*)NOT_INITALIZED; // stack items

#ifdef STACK_USE_SELF_HASH
	int right_hash_ref = NOT_INITALIZED;
    uint64_t self_hash = NOT_INITALIZED;
#endif

#ifdef STACK_USE_DATA_HASH
    uint64_t data_hash = NOT_INITALIZED;
#endif

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
