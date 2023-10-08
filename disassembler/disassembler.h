#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H
//#########################################################################################################

#include "../include/common.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>

//#########################################################################################################

#define check_cmd(code) ((code) & 0b00001111)
#define check_param_type(code) ((code) & 0b11110000)

//#########################################################################################################

struct DisAsmFile
{
  FILE *bytecode_file;
  size_t bytecode_file_size;

  char *bytecode_buffer;
  size_t commands_num;
  
  FILE *cmd_file;

  int err_code;
};

enum paramtypes
{
  immediate_const_type          = 1 << 4,
  register_type                 = 1 << 5,
  no_param_type                 =      0,
};

enum executionstatus
{
  execution_success             =      0,
  buffer_allocation_error       = 1 << 0,
  words_arr_allocation_error    = 1 << 1,
  commands_arr_allocation_error = 1 << 2,
  open_cmd_file_error           = 1 << 3,
  write_bytecode_file_error     = 1 << 4,
  invalid_command_error         = 1 << 29,
  invalid_param_error           = 1 << 30,
};

//#########################################################################################################

#endif
