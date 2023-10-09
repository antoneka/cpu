#include <assert.h>
#include <stdlib.h>
#include "stack.h"
#include "stack_debug.h"
#include "hash_rot13.h"

//-----------------------------------------------------------------------------------------------------------

enum ShiftDir
{
  LEFT_SHIFT  = 1,
  RIGHT_SHIFT = 2,
};

//-----------------------------------------------------------------------------------------------------------

static void appendCanaries(Stack *stk);

static void fillWithPoison(Stack *stk, size_t src, size_t dst);

static void shiftArray(Stack *stk, ShiftDir direction);

//-----------------------------------------------------------------------------------------------------------

static void appendCanaries(Stack *stk)
{
  canary_t *canary_left = (canary_t*)stk->data;
  canary_t *canary_right = (canary_t*)((elem_t*)(canary_left + 1) + stk->capacity);

  *canary_left = CANARY_VALUE;
  *canary_right = CANARY_VALUE;
}

//-----------------------------------------------------------------------------------------------------------

static void fillWithPoison(Stack *stk, size_t src, size_t dst)
{
  for (size_t elem_cnt = src; elem_cnt < dst; elem_cnt++) 
    {
      stk->data[elem_cnt] = POISON_VALUE;
    }
}

//-----------------------------------------------------------------------------------------------------------

static void shiftArray(Stack *stk, ShiftDir direction)
{
  canary_t* data_canary_cast = (canary_t*)stk->data;

  if (direction == LEFT_SHIFT)
    {
      data_canary_cast -= 1;
    }
  else if (direction == RIGHT_SHIFT)
    {
      data_canary_cast += 1;
    }

  stk->data = (elem_t*)data_canary_cast;
}

//-----------------------------------------------------------------------------------------------------------

ExecStatus stackCtor(Stack *stk, size_t initial_cap, const char *var_name, 
                     const char *file_name, size_t line_num, const char *func_name)
{
  if (stk == nullptr)
    { 
      fprintf(stderr, "Stack pointer passed to the constructor is NULL\n");

      return STACK_PTR_IS_NULL;
    }

  if (stk->init_status == CONSTRUCTED)
    {
      return STACK_ALREADY_CONSTRUCTED;
    }

  stk->capacity = initial_cap;
  stk->size = 0;
  stk->err_code = 0;

  stk->var = var_name;
  stk->file = file_name;
  stk->line = line_num;
  stk->func = func_name;

  stk->data = (elem_t*)calloc(stk->capacity * sizeof(elem_t) 
                              ON_DEBUG(+ 2 * sizeof(canary_t)), sizeof(char));

  if (stk->data == nullptr)
    {
      stk->err_code |= STACK_ALLOCATION_ERROR;

      return STACK_ALLOCATION_ERROR;
    }

  ON_DEBUG
    (
      stk->stack_left_can = CANARY_VALUE;
      stk->stack_right_can = CANARY_VALUE;

      appendCanaries(stk);

      shiftArray(stk, RIGHT_SHIFT);
    )

  fillWithPoison(stk, 0, stk->capacity);

  stk->init_status = CONSTRUCTED;

  ON_DEBUG
    (
     stk->hash = hashCalc(stk);
    )

  STACK_ASSERT(stk);

  return EXECUTION_SUCCESS;
} 

//-----------------------------------------------------------------------------------------------------------

ExecStatus stackPush(Stack *stk, elem_t value)
{
  STACK_ASSERT(stk);

  if (stk->size == stk->capacity)
    {
      ExecStatus resize_status = stackResize(stk, stk->capacity * RESIZE_COEF);

      if (resize_status != EXECUTION_SUCCESS)
        {
          stk->err_code |= resize_status;

          return resize_status;
        }
    }
  
  stk->data[stk->size++] = value;

  ON_DEBUG
    (
      stk->hash = hashCalc(stk);
    )

  STACK_ASSERT(stk);

  return EXECUTION_SUCCESS;
}

//-----------------------------------------------------------------------------------------------------------

ExecStatus stackPop(Stack *stk, elem_t *return_value)
{
  STACK_ASSERT(stk);

  if (return_value == nullptr)
    {
      stk->err_code |= POPVALUE_PTR_IS_NULL;

      return POPVALUE_PTR_IS_NULL;
    }

  *return_value = stk->data[--stk->size];

  stk->data[stk->size] = POISON_VALUE;

  ON_DEBUG
    (
      stk->hash = hashCalc(stk);
    )

  if (stk->size * MAX_DIFF_COEF < stk->capacity)
    {
      ExecStatus resize_status = stackResize(stk, stk->capacity / RESIZE_COEF);

      if (resize_status != EXECUTION_SUCCESS)
        {
          stk->err_code |= resize_status;

          return resize_status;
        }
    }

  STACK_ASSERT(stk);

  return EXECUTION_SUCCESS;
}

//-----------------------------------------------------------------------------------------------------------

ExecStatus stackResize(Stack *stk, size_t new_capacity)
{
  STACK_ASSERT(stk);

  stk->capacity = new_capacity > 0 ? new_capacity : STANDARD_CAPACITY;

  ON_DEBUG
    (
      shiftArray(stk, LEFT_SHIFT);
    )

  elem_t *data_tmp = (elem_t*)realloc(stk->data, stk->capacity * sizeof(elem_t) 
                                      ON_DEBUG(+ 2 * sizeof(canary_t)));

  if (data_tmp == nullptr)
    {
      stk->err_code |= STACK_REALLOCATION_ERROR;

      return STACK_REALLOCATION_ERROR;
    }

  stk->data = data_tmp;

  ON_DEBUG
    (
      appendCanaries(stk);

      shiftArray(stk, RIGHT_SHIFT);
    )

  fillWithPoison(stk, stk->size, stk->capacity);

  ON_DEBUG
    (
      stk->hash = hashCalc(stk);
    )

  STACK_ASSERT(stk);

  return EXECUTION_SUCCESS;
}

//-----------------------------------------------------------------------------------------------------------

ON_DEBUG(
unsigned int hashCalc(Stack *stk)
{
  assert(stk != nullptr);

  unsigned int old_hash = stk->hash;
  unsigned int new_hash = 0;

  stk->hash = 0;

  new_hash = hashRot13((char*)stk, sizeof(*stk));

  shiftArray(stk, LEFT_SHIFT);

  new_hash += hashRot13((char*)(stk->data), stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

  shiftArray(stk, RIGHT_SHIFT);

  stk->hash = old_hash;

  return new_hash;
}
)

//-----------------------------------------------------------------------------------------------------------

ExecStatus stackDtor(Stack *stk)
{
  if (stk == nullptr)
    {
      fprintf(stderr, "Stack pointer passed to the destructor is NULL\n");

      return STACK_PTR_IS_NULL;
    }

  if (stk->init_status == DESTRUCTED)
    {
      return STACK_ALREADY_DESTRUCTED;
    }

  stk->capacity = 0;
  stk->size = 0;
  stk->err_code = 0;

  stk->var = nullptr;
  stk->file = nullptr;
  stk->line = 0;
  stk->func = nullptr;

  ON_DEBUG
    (
      stk->stack_left_can = POISON_VALUE;
      stk->stack_right_can = POISON_VALUE;
      stk->hash = POISON_VALUE;

      shiftArray(stk, LEFT_SHIFT);
    )

  free(stk->data);
  stk->data = nullptr;

  stk->init_status = DESTRUCTED;

  return EXECUTION_SUCCESS;
}

