#include "stack_canary.h"

StackResult SetCanary(Stack* stack)
{
	stack->left_canary = CANARY_VALUE;
	stack->right_canary = CANARY_VALUE;

	int* left_canary_ptr  = (int*)((char*)stack->data - CANARY_SIZE);
	int* right_canary_ptr = (int*)((char*)stack->data + stack->cell_size * stack->capacity);

	*left_canary_ptr = CANARY_VALUE;
	*right_canary_ptr = CANARY_VALUE;

	return STACK_OK;
}

StackResult VerifyCanary(Stack* stack)
{
	if (stack->left_canary != CANARY_VALUE)
		return STACK_MEM_CORRUPTED;
	if (stack->right_canary != CANARY_VALUE)
		return STACK_MEM_CORRUPTED;

	int* left_canary_ptr = (int*)((char*)stack->data - CANARY_SIZE);
	int* right_canary_ptr = (int*)((char*)stack->data + stack->cell_size * stack->capacity);

	if (*left_canary_ptr != CANARY_VALUE)
		return STACK_MEM_CORRUPTED;
	if (*right_canary_ptr != CANARY_VALUE)
		return STACK_MEM_CORRUPTED;

	return STACK_OK;
}