#ifndef STACK_DEBUG_H
#define STACK_DEBUG_H

#include <stdio.h>
#include "stack.h"

//-----------------------------------------------------------------------------------------------------------

#define ELEM_MODIFIER "%d"
#define CANARY_MODIFIER "%llu"

//-----------------------------------------------------------------------------------------------------------

#define STACK_DUMP(stk, log_file) stackDump(stk, __FILE__, __LINE__, __func__, log_file)
#define PRINT_ERRORS(stk, log_file) printErrors(stk, __FILE__, __LINE__, __func__, log_file)

#define STACK_ASSERT(stk)                                 \
  do                                                      \
    {                                                     \
      int check_status = stackCheck(stk);                 \
                                                          \
      if (check_status != 0)                              \
        {                                                 \
          FILE *log_file = fopen("stack_log.txt", "a+");  \
                                                          \
          STACK_DUMP(stk, log_file);                      \
          PRINT_ERRORS(stk, log_file);                    \
                                                          \
          if (check_status != STACK_PTR_IS_NULL)          \
            {                                             \
              stackDtor(stk);                             \
            }                                             \
                                                          \
          if (log_file)                                   \
            {                                             \
              fclose(log_file);                           \
            }                                             \
                                                          \
          exit(EXIT_FAILURE);                             \
        }                                                 \
    }                                                     \
  while(0)                                        

//-----------------------------------------------------------------------------------------------------------

void stackDump(Stack *stk, const char *file_name, size_t line_num, 
               const char *func_name, FILE *log_file);

int stackCheck(Stack *stk);

void printErrors(Stack *stk, const char *file_name, size_t line_num, 
                  const char *func_name, FILE *log_file);

#endif // STACK_DEBUG_H
