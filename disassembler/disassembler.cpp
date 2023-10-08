#include "disassembler.h"

static int initByteCodeBuffer(DisAsmFile *disasmfile);

static void splitByteCodeBuffer(DisAsmFile *disasmfile);

int disAsmFileCtor(DisAsmFile *disasmfile)
{
  assert(disasmfile != nullptr);

  disasmfile->bytecode_file = fopen("bytecode_file.txt", "r");

  if (disasmfile->bytecode_file == nullptr)
    {
      return -1;
    }

  disasmfile->cmd_file = fopen("cmd_file.txt", "w");

  if (disasmfile->cmd_file == nullptr)
    {
      return -1;
    }

  disasmfile->bytecode_buffer = nullptr;
  disasmfile->bytecode_file_size = 0;
  disasmfile->err_code = 0;

  int init_buffer_status = initByteCodeBuffer(disasmfile);

}

static int initByteCodeBuffer(DisAsmFile *disasmfile)
{
  disasmfile->bytecode_file_size = getFileSize("bytecode_file.txt");

  disasmfile->bytecode_buffer = (char*)calloc(disasmfile->bytecode_file_size, sizeof(char));

  if (disasmfile->bytecode_buffer == nullptr)
    {
      // disasmfile->err_code |= BUFFER_ALLOCATION_ERROR;

      return -1;
    }

  fread(disasmfile->bytecode_buffer, sizeof(char), disasmfile->bytecode_file_size, disasmfile->cmd_file);

  splitByteCodeBuffer(disasmfile);

  return 0;
}

static void splitByteCodeBuffer(DisAsmFile *disasmfile)
{
  for (size_t symb_cnt = 1; symb_cnt < disasmfile->bytecode_file_size; symb_cnt++) 
    {
      if (disasmfile->bytecode_buffer[symb_cnt] == '\n')
        {
          disasmfile->bytecode_buffer[symb_cnt] = '\0';

          disasmfile->commands_num++;
        }
    }
}

void outputDisAsmFile(DisAsmFile *diasmfile)
{
   
}
