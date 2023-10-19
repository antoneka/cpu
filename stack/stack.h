#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include "../include/common.h"

#define DEBUG

//-----------------------------------------------------------------------------------------------------------
#define STACK_CTOR(stk, capacity) stackCtor((stk), capacity, #stk, __FILE__, __LINE__, __func__)

//-----------------------------------------------------------------------------------------------------------

#ifdef DEBUG
#define ON_DEBUG(code, ...) code __VA_ARGS__
#else
#define ON_DEBUG(code) 
#endif

//-----------------------------------------------------------------------------------------------------------

typedef int elem_t;
typedef unsigned long long canary_t;

const size_t STANDARD_CAPACITY = 8;
const size_t RESIZE_COEF       = 2;
const size_t MAX_DIFF_COEF     = 3;

const elem_t POISON_VALUE      = (elem_t)0xDADD0FAC;
const canary_t CANARY_VALUE    = (canary_t)0xBADC0FFEE;

enum StackStatus
{
  DESTRUCTED  = 0,
  CONSTRUCTED = 1,
};

//-----------------------------------------------------------------------------------------------------------

struct Stack
{
  ON_DEBUG(
  canary_t stack_left_can;
  )

  int init_status;
  
  size_t capacity; 
  size_t size;
  elem_t *data;

  const char *var;
  const char *file; 
  size_t line; 
  const char *func;

  int err_code;

  ON_DEBUG(
  unsigned int hash;
  canary_t stack_right_can;
  )
};

//-----------------------------------------------------------------------------------------------------------

int stackCtor(Stack *stk, size_t initial_cap, const char *var_name, 
                     const char *file_name, size_t line_num, const char *func_name);

int stackPush(Stack *stk, elem_t value);

int stackPop(Stack *stk, elem_t *return_value);

int stackResize(Stack *stk, size_t new_capacity);

ON_DEBUG(
unsigned int hashCalc(Stack *stk);
)

int stackDtor(Stack *stk);

#endif // STACK_H
