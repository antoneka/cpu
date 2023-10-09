#ifndef STACK_H
#define STACK_H

#include <stdio.h>

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

enum ExecStatus
{
  EXECUTION_SUCCESS            = 1 << 0,
  STACK_ALLOCATION_ERROR       = 1 << 1,
  STACK_CAPACITY_ERROR         = 1 << 2,
  STACK_SIZE_ERROR             = 1 << 3,
  LEFT_CANARY_STACK_CORRUPTED  = 1 << 4,
  RIGHT_CANARY_STACK_CORRUPTED = 1 << 5,
  LEFT_CANARY_DATA_CORRUPTED   = 1 << 6,
  RIGHT_CANARY_DATA_CORRUPTED  = 1 << 7,
  STACK_REALLOCATION_ERROR     = 1 << 8,
  STACK_ALREADY_CONSTRUCTED    = 1 << 9,
  STACK_ALREADY_DESTRUCTED     = 1 << 10,
  STACK_PTR_IS_NULL            = 1 << 11,
  POPVALUE_PTR_IS_NULL         = 1 << 12,
  HASH_CORRUPTED               = 1 << 13,
};

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

ExecStatus stackCtor(Stack *stk, size_t initial_cap, const char *var_name, 
                     const char *file_name, size_t line_num, const char *func_name);

ExecStatus stackPush(Stack *stk, elem_t value);

ExecStatus stackPop(Stack *stk, elem_t *return_value);

ExecStatus stackResize(Stack *stk, size_t new_capacity);

ON_DEBUG(
unsigned int hashCalc(Stack *stk);
)

ExecStatus stackDtor(Stack *stk);

#endif // STACK_H
