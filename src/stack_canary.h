#ifndef STACK_CANARY_H
#define STACK_CANARY_H

#include "stack.h"

StackResult SetCanary(Stack* stack);
StackResult VerifyCanary(Stack* stack);

#endif