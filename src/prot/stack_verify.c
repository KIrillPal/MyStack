#include "stack_verify.h"

StackResult VerifyInitalize(Stack* stack)
{
	if (!stack)
		return STACK_NOT_INITALIZED;
	if (stack->init_status == NOT_INITALIZED)
		return STACK_NOT_INITALIZED;

	return STACK_OK;
}

StackResult VerifyParams(Stack * stack)
{
	if (stack->init_status != IS_INITALIZED)
		return STACK_MEM_CORRUPTED;

	if (stack->cell_size == NOT_INITALIZED ||
		stack->size      == NOT_INITALIZED ||
		stack->capacity  == NOT_INITALIZED ||
		stack->data      == (void*)NOT_INITALIZED)
		return STACK_MEM_CORRUPTED;
		
	if (stack->size > stack->capacity)
		return STACK_MEM_CORRUPTED;
	if (!stack->data)
		return STACK_MEM_CORRUPTED;

	return STACK_OK;
}