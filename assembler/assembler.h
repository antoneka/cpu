#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

struct Token
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

  Token *tokens_arr;
  size_t tokens_num;

  int err_code;
};


enum CodeMasks
{
  IMMEDIATE_CONST_MASK = 1 << 4,
  REGISTER_MASK        = 1 << 5,

  CMD_TYPE_MASK        = 0b11110000,
  CMD_MASK             = 0b00001111,
};

int asmFileCtor(AsmFile *asmfile);

int asmFileDtor(AsmFile *asmfile);

void outputAsmFile(AsmFile *asmfile);

#endif // ASSEMBLER_H
