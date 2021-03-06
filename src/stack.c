#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "stack_utils.h"

#ifdef STACK_DUMPING
#define DUMP(stack) Dump(stack)
#else
#define DUMP(stack) 
#endif

#define ABORT_SILENT(error_code) { return error_code; }

#define ABORT_LOUD(error_code) {                                       \
	printf("Error in %s at line %d: ", __PRETTY_FUNCTION__, __LINE__); \
	StackPrintErrDescription(error_code, stdout);					   \
	printf("\n"); 							                    	   \
	abort(); 	                                                       \
}

#ifdef STACK_SILENT
#define ABORT(error_code) { DUMP(stack); ABORT_SILENT(error_code) }
#else 
#define ABORT(error_code) { DUMP(stack); ABORT_LOUD(error_code) }
#endif

#define CHECK(function) {                              \
	StackResult error_code = function;                 \
	if (error_code != STACK_OK) 				       \
		ABORT(error_code)                              \
}												       
												       
#define VERIFY(stack, error_base) {				       \
	StackResult error_code = Verify(stack);		       \
												       \
	if (error_code == STACK_MEM_CORRUPTED)		       \
		ABORT(STACK_MEM_CORRUPTED);				       \
												       \
													   \
	if (error_code != STACK_OK)					       \
		ABORT(MERGE_RESULTS(error_base, error_code));  \
}



static void StackDump(Stack* stack)
{
#ifdef STACK_SILENT
	if (Verify(stack) == STACK_OK)
		Dump(stack);
#else
	VERIFY(stack, STACK_DUMP_ERROR);
	CHECK (Dump(stack))
#endif
}

StackResult StackRealloc(Stack* stack, size_t new_capacity)
{
	VERIFY(stack, STACK_REALLOC_ERROR);
	CHECK (Realloc(stack, new_capacity))
	return STACK_OK;
}

StackResult StackCtor(Stack* stack, size_t cell_size)
{
	CHECK (Ctor(stack, cell_size))
	return STACK_OK;
}

StackResult StackDtor(Stack* stack)
{
	VERIFY(stack, STACK_DTOR_ERROR);
	CHECK (Dtor(stack))
	return STACK_OK;
}

StackResult StackPush(Stack* stack, void* item)
{
	VERIFY(stack, STACK_PUSH_ERROR);
	CHECK (Push(stack, item));
	return STACK_OK;
}

StackResult StackTop(Stack* stack, void* item)
{
	VERIFY(stack, STACK_TOP_ERROR);
	CHECK (Top(stack, item));
	return STACK_OK;
}

StackResult StackPop(Stack* stack)
{
	VERIFY(stack, STACK_POP_ERROR);
	CHECK (Pop(stack));
	return STACK_OK;
}

StackResult StackPop(Stack* stack, void* item)
{
	VERIFY(stack, STACK_POP_ERROR);
	CHECK (Pop(stack, item));
	return STACK_OK;
}
