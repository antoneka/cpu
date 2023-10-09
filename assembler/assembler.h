#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//#########################################################################################################

#include "../include/common.h"
#include <assert.h>
#include <ctype.h>
#include <string.h> 
#include <stdlib.h>

//#########################################################################################################

#define CHECK_CMD(code) ((code) & 0b00001111)
#define CHECK_PARAM_TYPE(code) ((code) & 0b11110000)

//#########################################################################################################

struct Command
{
  int code;
  int param;
};

struct AsmFile
{
  FILE *cmd_file;
  size_t cmd_file_size;

  char *command_buffer;
  
  char **word_arr;
  size_t words_num;

  Command *command_arr;
  size_t commands_num;

  int err_code;
};

//#########################################################################################################

int asmFileCtor(AsmFile *asmfile);

int asmFileDtor(AsmFile *asmfile);

int outputAsmFile(AsmFile *asmfile);

#endif // ASSEMBLER_H
