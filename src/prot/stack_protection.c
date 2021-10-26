#include "stack_protection.h"

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


#ifdef STACK_USE_CANARY

StackResult SetCanary(Stack* stack)
{
	stack->left_canary = CANARY_VALUE;
	stack->right_canary = CANARY_VALUE;

	int* left_canary_ptr  = (int*)((uint8_t*)stack->data - CANARY_SIZE);
	int* right_canary_ptr = (int*)((uint8_t*)stack->data + stack->cell_size * stack->capacity);

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

	int* left_canary_ptr = (int*)((uint8_t*)stack->data - CANARY_SIZE);
	int* right_canary_ptr = (int*)((uint8_t*)stack->data + stack->cell_size * stack->capacity);

	if (*left_canary_ptr != CANARY_VALUE)
		return STACK_MEM_CORRUPTED;
	if (*right_canary_ptr != CANARY_VALUE)
		return STACK_MEM_CORRUPTED;

	return STACK_OK;
}

#endif


#ifdef STACK_USE_HASH

uint64_t GetHash(void * start_ptr, void * end_ptr)
{
    uint64_t hash = 0;
    for (uint8_t* ptr = (uint8_t*)start_ptr; ptr < end_ptr; ++ptr)
    {
        hash += *ptr;
    }
    return hash;
}

uint64_t GetSelfHash(Stack * stack)
{
    return GetHash(&stack->left_hash_ref, &stack->right_hash_ref);
}

uint64_t GetDataHash (Stack * stack)
{
    return GetHash(stack->data, (uint8_t*)stack->data + stack->cell_size * stack->size);
}

#ifdef STACK_USE_SELF_HASH

StackResult SetSelfHash(Stack * stack)
{
    stack->left_hash_ref  = HASH_REF_VALUE;
    stack->right_hash_ref = HASH_REF_VALUE;

    stack->self_hash = GetSelfHash(stack);
    return STACK_OK;
}

StackResult VerifySelfHash(Stack * stack)
{
    uint64_t self_hash_now = GetSelfHash(stack);

    if (stack->left_hash_ref != HASH_REF_VALUE)
        return STACK_MEM_CORRUPTED;
    if (stack->right_hash_ref != HASH_REF_VALUE)
        return STACK_MEM_CORRUPTED;

    if (self_hash_now != stack->self_hash)
        return STACK_MEM_CORRUPTED;

    return STACK_OK;
}

#endif

#ifdef STACK_USE_DATA_HASH

StackResult SetDataHash(Stack * stack)
{
    stack->data_hash = GetDataHash(stack);
    return STACK_OK;
}

StackResult VerifyDataHash(Stack * stack)
{
    uint64_t data_hash_now = GetDataHash(stack);

    if (data_hash_now != stack->data_hash)
        return STACK_MEM_CORRUPTED;

    return STACK_OK;
}

#endif

StackResult RemoveFromHash(uint64_t * hash, void * elem, size_t size)
{
    uint8_t * elem_byte = (uint8_t*)elem;
    for (int i = 0; i < size; ++i)
    {
        *hash -= *elem_byte;
        elem_byte++;
    }
   
    return STACK_OK;
}

StackResult AddToHash(uint64_t * hash, void * elem, size_t size)
{
    uint8_t * elem_byte = (uint8_t*)elem;
    for (int i = 0; i < size; ++i)
    {
        *hash += *elem_byte;
        elem_byte++;
    }

    return STACK_OK;
}

#endif