#include "cpu.h"
#include "../include/common.h"
#include <stdlib.h>

static int initBuffer(CPU *cpu);
int getParam(CPU *cpu, elem_t **param);

int spuCtor(CPU *cpu)
{
  cpu->bin_file = fopen("asm_file.bin", "rb");

  if (cpu->bin_file == nullptr)
    {
      return ;
    }

  STACK_CTOR(cpu->stk, STANDARD_CAPACITY);

  cpu->bin_file_size = getFileSize(cpu->bin_file);
  cpu->pc = 0;
  cpu->err_code = 0;

  int buffer_init_status = initBuffer(cpu);

  if (buffer_init_status != EXECUTION_SUCCESS)
    {
      return buffer_init_status;
    }
}

static int initBuffer(CPU *cpu)
{
  cpu->buff_size = cpu->bin_file_size / sizeof(int);

  cpu->buffer = (int*)calloc(cpu->buff_size, sizeof(int));

  if (cpu->buffer == nullptr)
    {
      return BUFFER_ALLOCATION_ERROR;
    }

  fread(cpu->buffer, sizeof(int), cpu->buff_size, cpu->bin_file);

  return EXECUTION_SUCCESS;
}

#define DEF_CMD(cmd, params, ...)                             \
        case cmd:                                             \
          {                                                   \
            if (params)                                       \
              {                                               \
                elem_t param = 0;                             \
                elem_t *param_ptr = &param;                   \
                                                              \
                int get_param_status = getParam(cpu, &param); \
                                                              \
                if (get_param_status != EXECUTION_SUCCESS)    \
                  {                                           \
                    cpu->err_code |= INVALID_PARAM_ERROR;     \
                                                              \
                    return INVALID_PARAM_ERROR;               \
                  }                                           \
              }                                               \
                                                              \
            __VA_ARGS__;                                      \
          }

int executeCommands(CPU *cpu)
{
  Stack *stk = cpu->stk;
  int *buffer = cpu->buffer;

  int read_cmd = 0;

  while ((read_cmd = GET_CMD(buffer[cpu->pc])) != HLT) 
    {
      switch (read_cmd)
      {
        #include "../include/commands.h"
      }

      cpu->pc++;
    }
}

int getParam(CPU *cpu, elem_t **param)
{
  int *buffer = cpu->buffer;

  if (GET_PARAM_TYPE(buffer[cpu->pc]) == IMMEDIATE_CONST_TYPE)
    {
      **param = buffer[++cpu->pc];
    }

  else if (GET_PARAM_TYPE(buffer[cpu->pc]) == REGISTER_TYPE)
    {
      int reg_idx = buffer[++cpu->pc];
      *param = &cpu->regs[reg_idx];
    }

  else 
    {
      return INVALID_PARAM_ERROR;
    }

  return EXECUTION_SUCCESS;
}

