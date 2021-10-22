#include "stack_hash.h"

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