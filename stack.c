#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

static void* GetEndPtr(Stack* stack, size_t offset = 0)
{
	if (!stack)
		return stack;

	return (void*)((char*)stack->data + (stack->size + offset) * stack->cell_size);
}

static void StackDump(Stack* stack)
{
	printf("DUMP:      %p\n", stack);
	printf("SIZE:      %ld\n" , stack->size);
	printf("CAPACITY:  %ld\n", stack->capacity);
	printf("CELL_SIZE: %ld\n" , stack->cell_size);
	printf("DATA:      %p\n", stack->data);
	printf("%d bytes: ", stack->size);

	int i;
	for (i = 0; i < stack->size; ++i)
	{
		printf("%lf ", ((double*)stack->data)[i]);
	}
	printf("\nDUMP finished\n\n");

}

int StackRealloc(Stack* stack, size_t new_capacity)
{
	size_t possible_cap = stack->capacity;
	
	while (possible_cap < new_capacity)
		possible_cap *= 2;

	while (possible_cap > 2 * new_capacity)
		possible_cap /= 2;

	if (possible_cap < 1)
		possible_cap = 1;;

	if (possible_cap != stack->capacity)
	{
		stack->data = realloc(stack->data, possible_cap * stack->cell_size);
		stack->capacity = possible_cap;
	}

	return stack->data ? STACK_OK : STACK_MEM_ERROR;
}



int StackCtor(Stack* stack, size_t cell_size)
{
	stack->data      = calloc(1, cell_size);
	stack->cell_size = cell_size;
	stack->size      = 0;
	stack->capacity  = 1;

	return stack->data ? STACK_OK : STACK_MEM_ERROR;
}

int StackDtor(Stack* stack)
{
	if (!stack || !stack->data)
		return STACK_NOT_CONSTRUCTED;

	free(stack->data);
	return STACK_OK;
}

int StackPush(Stack* stack, void* item)
{
	if (!stack)
		return STACK_NOT_CONSTRUCTED;
	if (!item)
		return STACK_ITEM_ERROR;

	int err = StackRealloc(stack, stack->size + 1);
	if (err) return err;

	void* errp = memcpy(GetEndPtr(stack), item, stack->cell_size);
	if (!errp) return STACK_MEM_ERROR;

	stack->size++;
	
	return STACK_OK;
}

int StackTop(Stack* stack, void* item)
{
	if (!stack)
		return STACK_NOT_CONSTRUCTED;
	if (!item)
		return STACK_ITEM_ERROR;
	if (stack->size == 0)
		return STACK_EMPTY_ERROR;

	void * errp = memcpy(item, GetEndPtr(stack, -1), stack->cell_size);
	if (!errp) return STACK_MEM_ERROR;

	return STACK_OK;
}

int StackPop(Stack* stack)
{
	if (!stack)
		return STACK_NOT_CONSTRUCTED;

	if (stack->size == 0)
		return STACK_EMPTY_ERROR;

	stack->size--;
	int err = StackRealloc(stack, stack->size);
	if (err) return err;

	return STACK_OK;
}

int StackPop(Stack* stack, void* item)
{
	int err = StackTop(stack, item);
	if (err) return err;

	return StackPop(stack);
}
