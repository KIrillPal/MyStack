#include <stdio.h>
#include <assert.h>
#include "stack.h"

typedef int cell_t;

#define PUSH(stack, val) { cell_t buff = val; StackPush(&stack, &buff); }
#define POPOUT(stack) { cell_t buff; StackPop(&stack, &buff); printf("%d\n", buff); }

int main()
{

    Stack stack;

    StackCtor(&stack, sizeof(cell_t));

	PUSH(stack, 12);
	PUSH(stack, 1);

	POPOUT(stack);
	POPOUT(stack);
	PUSH(stack, -1);
	PUSH(stack, 6);
	PUSH(stack, 6);
	PUSH(stack, 6);
	PUSH(stack, 6);
	PUSH(stack, 6);

	POPOUT(stack);
	POPOUT(stack);
	POPOUT(stack);
	POPOUT(stack);
	POPOUT(stack);
	POPOUT(stack);

    StackDtor(&stack);
}
