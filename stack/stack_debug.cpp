#include "stack_debug.h"
#include "hash_rot13.h"
#include <stdint.h>

void stackDump(Stack *stk, const char *file_name, size_t line_num, 
               const char *func_name, FILE *log_file)
{
  if (stk == nullptr)
    {
      return;
    }

  if (log_file == nullptr)
    {
      log_file = stderr;
    }

  fprintf(log_file, "Stack [%p] %s is declared in %s(%zu) %s() called from %s(%zu) %s()\n",
                     stk, stk->var, stk->file, stk->line, stk->func, file_name, line_num, func_name);

  fprintf(log_file, "{\n"
                    "\tsize = %zu\n"
                    "\tcapacity = %zu\n"
                    "\tdata[%p]\n"
                    "\t{\n", stk->size, stk->capacity, stk->data);

  for (size_t stk_elem = 0; stk_elem < stk->capacity; stk_elem++)
    {
      if (stk_elem < stk->size)
        {
          fprintf(log_file, "\t\t*[%zu] = " ELEM_MODIFIER "\n", stk_elem, stk->data[stk_elem]);
        }
      else 
        {
          fprintf(log_file, "\t\t [%zu] = " ELEM_MODIFIER " (POISON)\n", stk_elem, stk->data[stk_elem]);
        } 
    }
  
  fprintf(log_file, "\t}\n"
                    "}\n\n");

  fprintf(log_file, "==================================================================================\n\n");
}

//-----------------------------------------------------------------------------------------------------------

int stackCheck(Stack *stk)
{
  if (stk == nullptr)
    {
      return STACK_PTR_IS_NULL;
    }

  if (stk->data == nullptr)
    {
      stk->err_code |= STACK_ALLOCATION_ERROR;
    }

  if (stk->capacity == SIZE_MAX)
    {
      stk->err_code |= STACK_CAPACITY_ERROR;
    }

  if (stk->size == SIZE_MAX || stk->size > stk->capacity)
    {
      stk->err_code |= STACK_SIZE_ERROR;
    }

  ON_DEBUG
    (
      if (stk->stack_left_can != CANARY_VALUE)
        {
          stk->err_code |= LEFT_CANARY_STACK_CORRUPTED;
        }

      if (stk->stack_right_can != CANARY_VALUE)
        {
          stk->err_code |= RIGHT_CANARY_STACK_CORRUPTED;
        }

      if (stk->data != nullptr && *((canary_t*)stk->data - 1) != CANARY_VALUE)
        {
          stk->err_code |= LEFT_CANARY_DATA_CORRUPTED;
        }

      if (stk->data != nullptr && *(canary_t*)(stk->data + stk->capacity) != CANARY_VALUE)
        {
          stk->err_code |= RIGHT_CANARY_DATA_CORRUPTED;
        }

      unsigned int correct_hash = hashCalc(stk);

      if (correct_hash != stk->hash)
        {
          stk->err_code |= HASH_CORRUPTED;
        }
    )

  return stk->err_code;
}

//-----------------------------------------------------------------------------------------------------------

void printErrors(Stack *stk, const char *file_name, size_t line_num, 
                 const char *func_name, FILE *log_file)
{
  if (stk == nullptr)
    {
      fprintf(log_file, "Stack pointer passed to the function %s():%zu from %s is NULL\n",
                         func_name, line_num, file_name);

      return;
    }

  if (log_file == nullptr)
    {
      log_file = stderr;
    }

  int errors = stk->err_code;

  if (errors & STACK_ALLOCATION_ERROR)
    {
      fprintf(log_file, "Failed to allocate memory for the data array\n\n");
    }

  if (errors & STACK_REALLOCATION_ERROR)
    {
      fprintf(log_file, "Failed to resize the data array\n\n");
    }

  if (errors & STACK_CAPACITY_ERROR)
    {
      fprintf(log_file, "Invalid capacity value\n\n");
    }

  if (errors & STACK_SIZE_ERROR)
    {
      fprintf(log_file, "Invalid size value\n\n");
    }

  if (errors & POPVALUE_PTR_IS_NULL)
    {
      fprintf(log_file, "Popped value pointer passed to the pop function is NULL\n\n");
    }

  ON_DEBUG
    (
      if (errors & LEFT_CANARY_STACK_CORRUPTED)
        {
          fprintf(log_file, "Left canary of the stack is corrupted\n"
                            "Current left canary value: " CANARY_MODIFIER "\n"
                            "Correct canary value: " CANARY_MODIFIER "\n\n", 
                            stk->stack_left_can, CANARY_VALUE);
        }

      if (errors & RIGHT_CANARY_STACK_CORRUPTED)
        {
          fprintf(log_file, "Right canary of the stack is corrupted\n"
                            "Current right canary value: " CANARY_MODIFIER "\n"
                            "Correct canary value: " CANARY_MODIFIER "\n\n", 
                            stk->stack_right_can, CANARY_VALUE);
        }

      if (errors & LEFT_CANARY_DATA_CORRUPTED)
        {
          fprintf(log_file, "Left canary of the data array is corrupted\n"
                            "Current left canary value: " CANARY_MODIFIER "\n"
                            "Correct canary value: " CANARY_MODIFIER "\n\n", 
                            *((canary_t*)stk->data - 1), CANARY_VALUE);
        }

      if (errors & RIGHT_CANARY_DATA_CORRUPTED)
        {
          fprintf(log_file, "Right canary of the data array is corrupted\n"
                            "Current right canary value: " CANARY_MODIFIER "\n"
                            "Correct canary value: " CANARY_MODIFIER "\n\n", 
                            *(canary_t*)(stk->data + stk->capacity), CANARY_VALUE);
        }

      if (errors & HASH_CORRUPTED)
        {
          fprintf(log_file, "The hash was corrupted\n"
                            "Current hash: %u\n"
                            "Correct hash: %u\n\n", stk->hash, hashCalc(stk));
        }
    )
}

