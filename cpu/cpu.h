#ifndef CPU_H
#define CPU_H

//#########################################################################################################

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "../stack/stack.h"
#include "../stack/stack_debug.h"
#include "../include/common.h"

//#########################################################################################################

const size_t REGS_NUM = 4;

struct CPU
{
  FILE *bin_file;
  size_t bin_file_size;

  Stack *stk;

  int *buffer;
  size_t buff_size;

  elem_t regs[REGS_NUM];

  size_t pc;
  int err_code;
};

//#########################################################################################################

int cpuCtor(CPU *cpu);

int executeCommands(CPU *cpu);

int cpuDtor(CPU *cpu);


#endif
