#include "cpu.h"
#include "../include/common.h"
#include <stdlib.h>

static int initBuffer(CPU *cpu);

int spuCtor(CPU *cpu)
{
  cpu->bin_file = fopen("bytecode_file", "rb");

  if (cpu->bin_file == nullptr)
    {
      return -1;
    }

  STACK_CTOR(cpu->stk, STANDARD_CAPACITY);

  cpu->pc = 0;
  cpu->err_code = 0;

  int buffer_init_status = initBuffer(cpu);

  if (buffer_init_status != EXECUTION_SUCCESS)
    {
      return -1;
    }
  

}

static int initBuffer(CPU *cpu)
{
  fread(&cpu->buf_size, sizeof(size_t), 1, cpu->bin_file);

  cpu->buffer = (int*)calloc(cpu->buf_size, sizeof(int));

  for (size_t i = 0; i < cpu->buf_size; i++)
    {
      fread(cpu->buffer + i, sizeof(int), 1, cpu->bin_file);
    }

  return EXECUTION_SUCCESS;
}

void executeCommands(CPU *cpu)
{
  Stack *stk = cpu->stk;
  int *buffer = cpu->buffer;

  cpu->reg_arr = (int*)calloc(4, sizeof(int));
  int *reg_arr = cpu->reg_arr;

  size_t buf_size = cpu->buf_size;

  int read_cmd = 0;

  while ((read_cmd = GET_CMD(buffer[cpu->pc])) != HLT) 
    {
      switch (read_cmd)
      {
        case PUSH:
          {
            int arg_type = 0;
            elem_t arg = getArg(reg_arr[cpu->pc], &arg_type);

            stackPush(stk, arg);
          }

        case POP:
          {
            int arg_type = 0;
            elem_t arg = getArg(reg_arr[cpu->pc], &arg_type);

            stackPush(stk, arg);
          }

        case ADD:
          {
            elem_t first_num = 0;
            elem_t second_num = 0;

            stackPop(stk, &first_num);
            stackPop(stk, &second_num);

            stackPush(stk, first_num + second_num);
          }
      }
    }
}


