#include "stack_errors.h"

void StackPrintErrDescription(StackResult err, FILE* outerr)
{
	switch (err)
	{
	case STACK_OK:
		fprintf(outerr, "All correct"); break;
	case STACK_NOT_INITALIZED:
		fprintf(outerr, "Given null stack pointer or not initalized stack"); break;
	case STACK_MEM_NOT_ALLOCATE:
		fprintf(outerr, "Couldn't allocate memory"); break;
	case STACK_BAD_ARGUMENT:
		fprintf(outerr, "Given empty argument pointer"); break;
	case STACK_EMPTY:
		fprintf(outerr, "Given empty stack"); break;
	case STACK_MEM_CORRUPTED:
		fprintf(outerr, "Sorry, stack memory is corruped"); break;

	case STACK_DUMP_ERROR:
		fprintf(outerr, "Dump crashed"); break;

	case STACK_PUSH_ERROR:
		fprintf(outerr, "Push crashed"); break;
	case STACK_PUSH_NOSTACK_ERROR:
		fprintf(outerr, "Couldn't push to not initalized stack"); break;
	case STACK_PUSH_NOALLOC_ERROR:
		fprintf(outerr, "Couldn't copy memory to stack"); break;
	case STACK_PUSH_BADITEM_ERROR:
		fprintf(outerr, "Couldn't read argument. Maybe null pointer"); break;

	case STACK_POP_ERROR:
		fprintf(outerr, "Pop crashed"); break;
	case STACK_POP_NOSTACK_ERROR:
		fprintf(outerr, "Couldn't pop from not initalized stack"); break;
	case STACK_POP_EMPTY_ERROR:
		fprintf(outerr, "Couldn't pop from empty stack"); break;
	case STACK_POP_BADITEM_ERROR:
		fprintf(outerr, "Couldn't set a value to an argument. Maybe null pointer"); break;
	case STACK_POP_NOALLOC_ERROR:
		fprintf(outerr, "Couldn't copy memory from stack"); break;

	case STACK_TOP_ERROR:
		fprintf(outerr, "Top crashed"); break;
	case STACK_TOP_NOSTACK_ERROR:
		fprintf(outerr, "Couldn't get top value from not initalized stack"); break;
	case STACK_TOP_EMPTY_ERROR:
		fprintf(outerr, "Couldn't get top value from empty stack"); break;
	case STACK_TOP_BADITEM_ERROR:
		fprintf(outerr, "Couldn't set a value to an argument. Maybe null pointer"); break;
	case STACK_TOP_NOALLOC_ERROR:
		fprintf(outerr, "Couldn't copy memory from stack"); break;

	case STACK_REALLOC_ERROR:
		fprintf(outerr, "Couldn't reallocate"); break;
	case STACK_REALLOC_NOSTACK_ERROR:
		fprintf(outerr, "Couldn't reallocate not initalized stack"); break;

	case STACK_CTOR_ERROR:
		fprintf(outerr, "Couldn't initalize stack"); break;

	case STACK_DTOR_ERROR:
		fprintf(outerr, "Couldn't destroy stack"); break;
	case STACK_DTOR_NOSTACK_ERROR:
		fprintf(outerr, "Couldn't destroy not initalized stack"); break;
	default:
		fprintf(outerr, "Unknown error"); break;
	}
	fprintf(outerr, ". Code: %x\n", err);
}