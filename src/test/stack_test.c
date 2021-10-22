#include <stdio.h>
#include <string.h>
#include "../stack.h"

typedef int cell_t;

#define PUSH(stack, val) { cell_t buff = val; StackPush(&stack, &buff); }
#define POPOUT(stack) { cell_t buff; StackPop(&stack, &buff); printf("%d\n", buff); }

int main()
{

    Stack stack;

    StackCtor(&stack, sizeof(cell_t));

	for (int i = 0; i < 10; ++i)
	{
		PUSH(stack, 1);
	}

	for (int i = 0; i < 10; ++i)
	{
		int d;
		StackPop(&stack, &d);
	}

	memset(&stack, '1', 4);

    StackDtor(&stack);
}
