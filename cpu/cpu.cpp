#include "cpu.h"

//#########################################################################################################

// static int initAsmBuffer(CPU *cpu);

static int getParam(CPU *cpu, elem_t **param);

//#########################################################################################################

int cpuCtor(CPU *cpu)
{
  assert(cpu != nullptr);

  cpu->bin_file = fopen("asm_file.bin", "rb");

  if (cpu->bin_file == nullptr)
    {
      return ASM_FILE_OPEN_ERROR;
    }

  cpu->stk = (Stack*)calloc(1, sizeof(Stack));
  
  if (cpu->stk == nullptr)
    {
      return -1;
    }

  STACK_CTOR(cpu->stk, STANDARD_CAPACITY);

  cpu->bin_file_size = getFileSize(cpu->bin_file);
  cpu->pc = 0;
  cpu->err_code = 0;

  int init_buffer_status = initAsmBuffer(&cpu->buffer, &cpu->buff_size, 
                                         cpu->bin_file, cpu->bin_file_size);

  if (init_buffer_status != EXECUTION_SUCCESS)
    {
      return init_buffer_status;
    }

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

/*
static int initAsmBuffer(CPU *cpu)
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
*/

//#########################################################################################################

#define DEF_CMD(cmd, params, ...)                                   \
          case cmd:                                                 \
            {                                                       \
              elem_t param = 0;                                     \
              elem_t *param_ptr = &param;                           \
                                                                    \
              if (params)                                           \
                {                                                   \
                  int get_param_status = getParam(cpu, &param_ptr); \
                                                                    \
                  if (get_param_status != EXECUTION_SUCCESS)        \
                    {                                               \
                      cpu->err_code |= INVALID_PARAM_ERROR;         \
                                                                    \
                      return INVALID_PARAM_ERROR;                   \
                    }                                               \
                }                                                   \
                                                                    \
              __VA_ARGS__;                                          \
                                                                    \
              break;                                                \
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

          default:
            {
              cpu->err_code |= INVALID_COMMAND_ERROR;

              fprintf(stderr, "err\n");
            }

        }

      cpu->pc++;
    }

  return EXECUTION_SUCCESS;
}

#undef DEF_CMD

//#########################################################################################################

static int getParam(CPU *cpu, elem_t **param)
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

//#########################################################################################################

int cpuDtor(CPU *cpu)
{
  if (cpu->bin_file)
    {
      fclose(cpu->bin_file);
      cpu->bin_file = nullptr;
    }

  stackDtor(cpu->stk);
  free(cpu->stk);
  cpu->stk = nullptr;

  free(cpu->buffer);
  cpu->buffer = nullptr;

  cpu->bin_file_size = 0;
  cpu->pc = 0;
  cpu->err_code = 0;
  cpu->buff_size = 0;

  for (size_t reg_cnt = 0; reg_cnt < REGS_NUM; reg_cnt++)
    {
      cpu->regs[reg_cnt] = 0;
    }

  return EXECUTION_SUCCESS;
}


