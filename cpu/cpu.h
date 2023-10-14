#ifndef CPU_H
#define CPU_H

#include "../stack/stack.h"
#include "../include/common.h"

struct CPU
{
  FILE *bin_file;
  Stack *stk;

  int *buffer;
  size_t buf_size;
  int *regs;

  size_t pc;
  int err_code;
};

#endif
