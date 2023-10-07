#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../include/common.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>


#define CHECK_CMD(code) ((code) & 0b00001111)
#define CHECK_PARAM_TYPE(code) ((code) & 0b11110000)


struct Command
{
  int code;
  int param;
};

struct AsmFile
{
  FILE *cmd_file;
  size_t cmd_file_size;

  char *buffer;
  
  char **words_arr;
  size_t words_num;

  Command *commands_arr;
  size_t commands_num;

  int err_code;
};

enum ParamTypes
{
  IMMEDIATE_CONST_TYPE          = 1 << 4,
  REGISTER_TYPE                 = 1 << 5,
  NO_PARAM_TYPE                 =      0,
};

enum ExecutionStatus
{
  EXECUTION_SUCCESS             =      0,
  BUFFER_ALLOCATION_ERROR       = 1 << 0,
  WORDS_ARR_ALLOCATION_ERROR    = 1 << 1,
  COMMANDS_ARR_ALLOCATION_ERROR = 1 << 2,
  OPEN_CMD_FILE_ERROR           = 1 << 3,
  WRITE_BYTE_CODE_FILE_ERROR    = 1 << 4,
  INVALID_COMMAND_ERROR         = 1 << 29,
  INVALID_PARAM_ERROR           = 1 << 30,
};


int asmFileCtor(AsmFile *asmfile);

int asmFileDtor(AsmFile *asmfile);

int outputAsmFile(AsmFile *asmfile);

#endif // ASSEMBLER_H
