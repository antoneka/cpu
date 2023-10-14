#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

//#########################################################################################################

#include "../include/common.h"
#include <assert.h>
#include <string.h> 
#include <stdlib.h>

//#########################################################################################################

struct DisAsmFile
{
  FILE *bytecode_file;
  size_t bytecode_file_size;

  char *bytecode_buffer;
  
  int err_code;
};

//#########################################################################################################

int disAsmFileCtor(DisAsmFile *disasmfile);

int outputDisAsmFile(DisAsmFile *disasmfile);

int disAsmFileDtor(DisAsmFile *disasmfile);

#endif
