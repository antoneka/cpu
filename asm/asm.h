#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//#########################################################################################################

#include "../include/common.h"
#include <assert.h>
#include <ctype.h>
#include <string.h> 
#include <stdlib.h>

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

  char *cmd_buff;
  
  char **word_arr;
  size_t words_num;

  Command *cmd_arr;
  size_t cmds_num;

  int err_code;
};

//#########################################################################################################

int asmFileCtor(AsmFile *asmfile);

int asmFileDtor(AsmFile *asmfile);

int outputAsmFile(AsmFile *asmfile);

#endif // ASSEMBLER_H
