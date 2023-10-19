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
  FILE *bin_file;
  size_t bin_file_size;

  int *buffer;
  size_t buff_size;
  
  int err_code;
};

//#########################################################################################################

int disAsmFileCtor(DisAsmFile *disasmfile);

int outputDisAsmFile(DisAsmFile *disasmfile);

int disAsmFileDtor(DisAsmFile *disasmfile);

#endif
