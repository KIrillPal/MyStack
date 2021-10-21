#ifndef STACK_UTILS_H
#define STACK_UTILS_H

#include "stack.h"

StackResult Ctor(Stack* stack, size_t cell_size);
StackResult Dtor(Stack* stack);
StackResult Realloc(Stack * stack, size_t new_capacity);

StackResult Push   (Stack * stack, void * item);
StackResult Top    (Stack * stack, void * item);
StackResult Pop    (Stack * stack, void * item);
StackResult Pop    (Stack * stack);

StackResult Verify (Stack * stack);
StackResult Dump   (Stack * stack);

#endif