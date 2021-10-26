#ifndef STACK_PROTECTION_H
#define STACK_PROTECTION_H

#include "../stack.h"

StackResult VerifyInitalize (Stack * stack);
StackResult VerifyParams    (Stack * stack);

// Canary

StackResult SetCanary(Stack* stack);
StackResult VerifyCanary(Stack* stack);

// Hash

uint64_t GetHash (void* start_ptr, void* end_ptr);

uint64_t GetSelfHash (Stack * stack);
uint64_t GetDataHash (Stack * stack);

StackResult SetSelfHash    (Stack * stack);
StackResult VerifySelfHash (Stack * stack);

StackResult SetDataHash    (Stack * stack);
StackResult VerifyDataHash (Stack * stack);

StackResult RemoveFromHash (uint64_t * hash, void * elem, size_t size);
StackResult AddToHash      (uint64_t * hash, void * elem, size_t size);

#endif
