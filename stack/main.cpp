#include <stdio.h>
#include "stack.h"
#include "stack_debug.h"

int main()
{
  Stack stk = {};

  int creation_status = STACK_CTOR(NULL, 10);

  if (creation_status == STACK_ALREADY_CONSTRUCTED)
    {
      fprintf(stderr, "Stack has already been constructed\n");

      return 1;
    }

  elem_t element = {};

  STACK_DUMP(&stk, stderr);

  stackPush(&stk, 2);

  STACK_DUMP(&stk, stderr);

  stackPush(&stk, 5);

  STACK_DUMP(&stk, stderr);

  stackPush(&stk, 7);

  STACK_DUMP(&stk, stderr);

  stackPop(&stk, &element);

  STACK_DUMP(&stk, stderr);

  stackPop(&stk, &element);

  STACK_DUMP(&stk, stderr);

  stackPop(&stk, &element);

  STACK_DUMP(&stk, stderr);

  int destruction_status = stackDtor(&stk);

  if (destruction_status == STACK_ALREADY_DESTRUCTED)
    {
      fprintf(stderr, "Stack has already been destructed\n");

      return 1;
    }

  return 0;
}
