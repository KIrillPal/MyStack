#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "stack_utils.h"
#include "prot/stack_protection.h"


#define TRY(function) { StackResult err = function; if (err != STACK_OK) return err; }

static void* GetEndPtr(Stack* stack, size_t offset = 0)
{
	if (!stack)
		return stack;

	return (void*)((uint8_t*)stack->data + (stack->size + offset) * stack->cell_size);
}

StackResult Realloc(Stack * stack, size_t new_capacity)
{
	if (!stack || !stack->data)
		return STACK_REALLOC_NOSTACK_ERROR;

	size_t possible_cap = stack->capacity;

	while (possible_cap < new_capacity)
		possible_cap *= 2;

	while (possible_cap > 2 * new_capacity)
		possible_cap /= 2;

	if (possible_cap < 1)
		possible_cap = 1;

	if (possible_cap != stack->capacity)
	{

#ifdef STACK_USE_SELF_HASH
        RemoveFromHash(&stack->self_hash, &stack->data, sizeof(void*));
        RemoveFromHash(&stack->self_hash, &stack->capacity, sizeof(size_t));
#endif

		stack->capacity = possible_cap;

#ifdef STACK_USE_CANARY

		stack->data = (uint8_t*)(stack->data) - CANARY_SIZE;
		stack->data = realloc(stack->data, possible_cap * stack->cell_size + 2 * CANARY_SIZE);
		stack->data = (uint8_t*)(stack->data) + CANARY_SIZE;

		SetCanary(stack);
#else
		stack->data = realloc(stack->data, possible_cap * stack->cell_size);
#endif


#ifdef STACK_USE_SELF_HASH
        AddToHash(&stack->self_hash, &stack->data, sizeof(void*));
        AddToHash(&stack->self_hash, &stack->capacity, sizeof(size_t));
#endif

	}

	return stack->data ? STACK_OK : STACK_REALLOC_ERROR;
}



StackResult Ctor(Stack * stack, size_t cell_size)
{
	stack->cell_size = cell_size;
	stack->size = 0;
	stack->capacity = 1;
	stack->init_status = IS_INITALIZED;

#ifdef STACK_USE_CANARY

	stack->data = calloc(cell_size + 2 * CANARY_SIZE, 1);
	stack->data = (uint8_t*)(stack->data) + CANARY_SIZE;

	SetCanary(stack);
#else
	stack->data = calloc(1, cell_size);
#endif

#ifdef STACK_USE_SELF_HASH
    SetSelfHash(stack);
#endif

#ifdef STACK_USE_DATA_HASH
    SetDataHash(stack);
#endif

	return stack->data ? STACK_OK : STACK_CTOR_ERROR;
}

StackResult Dtor(Stack * stack)
{
	if (!stack || !stack->data)
		return STACK_DTOR_NOSTACK_ERROR;

#ifdef STACK_USE_CANARY
	free((void*)(((uint8_t*)stack->data) - CANARY_SIZE));
#else
	free(stack->data);
#endif

	stack->init_status = NOT_INITALIZED;
	stack->cell_size   = NOT_INITALIZED;
	stack->size        = NOT_INITALIZED;
	stack->capacity    = NOT_INITALIZED;
	stack->data        = (void*)NOT_INITALIZED;

#ifdef STACK_USE_SELF_HASH
    stack->left_hash_ref  = uint8_t(NOT_INITALIZED);
    stack->right_hash_ref = uint8_t(NOT_INITALIZED);
    stack->self_hash      = NOT_INITALIZED;
#endif

#ifdef STACK_USE_DATA_HASH
    stack->data_hash = NOT_INITALIZED;
#endif

	return STACK_OK;
}

StackResult Push(Stack * stack, void* item)
{
	if (!stack || !stack->data)
		return STACK_PUSH_NOSTACK_ERROR;
	if (!item)
		return STACK_PUSH_BADITEM_ERROR;

	TRY(Realloc(stack, stack->size + 1))

	void* errp = memcpy(GetEndPtr(stack), item, stack->cell_size);
	if (!errp) return STACK_PUSH_NOALLOC_ERROR;

#ifdef STACK_USE_DATA_HASH
    AddToHash(&stack->data_hash, GetEndPtr(stack), stack->cell_size);
#endif

#ifdef STACK_USE_SELF_HASH
    RemoveFromHash(&stack->self_hash, &stack->size, sizeof(size_t));
#endif

	stack->size++;

#ifdef STACK_USE_SELF_HASH
    AddToHash(&stack->self_hash, &stack->size, sizeof(size_t));
#endif

	return STACK_OK;
}

StackResult Top(Stack * stack, void* item)
{
	if (!stack)
		return STACK_TOP_NOSTACK_ERROR;
	if (!item)
		return STACK_TOP_BADITEM_ERROR;
	if (stack->size == 0)
		return STACK_TOP_EMPTY_ERROR;

	void* errp = memcpy(item, GetEndPtr(stack, -1), stack->cell_size);
	if (!errp) return STACK_TOP_NOALLOC_ERROR;

	return STACK_OK;
}

StackResult Pop(Stack * stack)
{
	if (!stack)
		return STACK_POP_NOSTACK_ERROR;

	if (stack->size == 0)
		return STACK_POP_EMPTY_ERROR;

#ifdef STACK_USE_DATA_HASH
    RemoveFromHash(&stack->data_hash, GetEndPtr(stack, -1), stack->cell_size);
#endif

#ifdef STACK_USE_SELF_HASH
    RemoveFromHash(&stack->self_hash, &stack->size, sizeof(size_t));
#endif

    stack->size--;

#ifdef STACK_USE_SELF_HASH
    AddToHash(&stack->self_hash, &stack->size, sizeof(size_t));
#endif

	TRY(Realloc(stack, stack->size))

	return STACK_OK;
}

StackResult Pop(Stack * stack, void* item)
{
	TRY(StackTop(stack, item))

	return StackPop(stack);
}

StackResult Verify(Stack* stack)
{
	TRY(VerifyInitalize(stack))
	TRY(VerifyParams(stack))

#ifdef STACK_USE_CANARY
	TRY(VerifyCanary(stack))
#endif

#ifdef STACK_USE_SELF_HASH
	TRY(VerifySelfHash(stack))
#endif

#ifdef STACK_USE_DATA_HASH
	TRY(VerifyDataHash(stack))
#endif

	return STACK_OK;
}

StackResult Dump(Stack* stack)
{
	printf("DUMP:         %p\n\n", stack);
#ifdef STACK_USE_CANARY
	printf("LEFT CANARY:  %x\n\n", stack->left_canary);
#endif

#ifdef STACK_USE_SELF_HASH
	printf("LHREF:        %x\n\n", stack->left_hash_ref);
#endif

	printf("INIT STATUS:  %x\n", stack->init_status);
	printf("SIZE:         %ld\n", stack->size);
	printf("CAPACITY:     %ld\n", stack->capacity);
	printf("CELL SIZE:    %ld\n", stack->cell_size);
	printf("DATA:         %p\n\n", stack->data);

#ifdef STACK_USE_SELF_HASH
	printf("RHREF:        %x\n", stack->right_hash_ref);
	printf("SELF HASH:    %x\n\n", stack->self_hash);
#endif

#ifdef STACK_USE_DATA_HASH
	printf("DATA HASH:    %x\n\n", stack->data_hash);
#endif

#ifdef STACK_USE_SELF_HASH
	printf("RIGHT CANARY: %x\n\n", stack->right_canary);
#endif

	printf("%d bytes: ", stack->size);

	int i;
	for (i = 0; i < stack->size; ++i)
	{
		printf("%lf ", ((double*)stack->data)[i]);
	}
	printf("\nDUMP finished\n\n");

	return ferror(stdout) ? STACK_DUMP_ERROR : STACK_OK;
}
