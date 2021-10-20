#include <stdio.h>
#include <assert.h>
#include "stack.h"

int main()
{
    Stack stack;
    StackCtor(&stack, sizeof(int));

    int d;
    assert(!StackPush(&stack, &(d = 12)));
    assert(!StackPush(&stack, &(d = -13344234)));

    int d1, d2;
    assert(!StackPop(&stack, &d1));
    assert(!StackPop(&stack, &d2));
    assert(!StackPush(&stack, &(d = d1 + d2)));
    assert(!StackPop(&stack, &d));
    printf("%d\n", d);
    StackDtor(&stack);
}
