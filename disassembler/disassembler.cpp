#include "disassembler.h"

//#########################################################################################################

static int initByteCodeBuffer(DisAsmFile *disasmfile);

static void splitByteCodeBuffer(DisAsmFile *disasmfile);

//#########################################################################################################

int disAsmFileCtor(DisAsmFile *disasmfile)
{
  assert(disasmfile != nullptr);

  disasmfile->bytecode_file = fopen("bytecode_file.txt", "r");

  if (disasmfile->bytecode_file == nullptr)
    {
      return OPEN_BYTECODE_FILE_ERROR;
    }

  disasmfile->bytecode_buffer = nullptr;
  disasmfile->bytecode_file_size = 0;
  disasmfile->err_code = 0;

  int init_buffer_status = initByteCodeBuffer(disasmfile);

  if (init_buffer_status != EXECUTION_SUCCESS)
    {
      return init_buffer_status;
    }

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

static int initByteCodeBuffer(DisAsmFile *disasmfile)
{
  disasmfile->bytecode_file_size = getFileSize("bytecode_file.txt");

  disasmfile->bytecode_buffer = (char*)calloc(disasmfile->bytecode_file_size, sizeof(char));

  if (disasmfile->bytecode_buffer == nullptr) 
    {
      disasmfile->err_code |= BUFFER_ALLOCATION_ERROR;

      return BUFFER_ALLOCATION_ERROR;
    }

  fread(disasmfile->bytecode_buffer, sizeof(char), disasmfile->bytecode_file_size, disasmfile->bytecode_file);

  splitByteCodeBuffer(disasmfile);

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

static void splitByteCodeBuffer(DisAsmFile *disasmfile)
{
  char sep[] = "\n";
  char *istr = strtok(disasmfile->bytecode_buffer, sep);

  while (istr != NULL)
    {
      istr = strtok(NULL, sep);
    }
}

//#########################################################################################################

int outputDisAsmFile(DisAsmFile *disasmfile)
{
  FILE *output_disasm = fopen("disasm_file.txt", "w");

  if (output_disasm == nullptr)
    {
      disasmfile->err_code |= WRITE_DISASM_FILE_ERROR;
    }

  for (size_t symb_cnt = 0; symb_cnt < disasmfile->bytecode_file_size;)
    {
      int cmd = 0;
      int param = 0;

      int nums_read = sscanf(disasmfile->bytecode_buffer + symb_cnt, "%d %d", &cmd, &param);

      if (nums_read == 0)
        {
          disasmfile->err_code |= INVALID_COMMAND_ERROR;

          return INVALID_COMMAND_ERROR;
        }

      if (CHECK_CMD(cmd) == PUSH)
        {
          if (nums_read == 1)
            {
              disasmfile->err_code |= INVALID_PARAM_ERROR;

              return INVALID_PARAM_ERROR;
            }

          fprintf(output_disasm, "%s ", "push");

          if (CHECK_PARAM_TYPE(cmd) == REGISTER_TYPE)
            {
              fprintf(output_disasm, "r%cx\n", param + 'a');
            }
          else if (CHECK_PARAM_TYPE(cmd) == IMMEDIATE_CONST_TYPE)
            {
              fprintf(output_disasm, "%d\n", param);
            }
        } 

      else if (CHECK_CMD(cmd) == POP)
        {
          if (nums_read == 1)
            {
              disasmfile->err_code |= INVALID_PARAM_ERROR;

              return INVALID_PARAM_ERROR;
            }

          fprintf(output_disasm, "%s ", "pop");

          if (CHECK_PARAM_TYPE(cmd) == REGISTER_TYPE)
            {
              fprintf(output_disasm, "r%cx\n", param + 'a');
            }
        } 

      else if (CHECK_CMD(cmd) == IN)
        {
          fprintf(output_disasm, "%s\n", "in");
        } 

      else if (CHECK_CMD(cmd) == HLT)
        {
          fprintf(output_disasm, "%s\n", "HLT");
        } 

      else if (CHECK_CMD(cmd) == OUT)
        {
          fprintf(output_disasm, "%s\n", "out");
        } 

      else if (CHECK_CMD(cmd) == ADD)
        {
          fprintf(output_disasm, "%s\n", "add");
        } 

      else if (CHECK_CMD(cmd) == SUB)
        {
          fprintf(output_disasm, "%s\n", "sub");
        } 

      else if (CHECK_CMD(cmd) == MUL)
        {
          fprintf(output_disasm, "%s\n", "mul");
        } 

      else if (CHECK_CMD(cmd) == DIV)
        {
          fprintf(output_disasm, "%s\n", "div");
        } 

      else if (CHECK_CMD(cmd) == SQRT)
        {
          fprintf(output_disasm, "%s\n", "sqrt");
        } 

      else if (CHECK_CMD(cmd) == SIN)
        {
          fprintf(output_disasm, "%s\n", "sin");
        } 

      else if (CHECK_CMD(cmd) == COS)
        {
          fprintf(output_disasm, "%s\n", "cos");
        } 

      symb_cnt += strlen(disasmfile->bytecode_buffer + symb_cnt) + 1;
    }

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

int disAsmFileDtor(DisAsmFile *disasmfile)
{
  if (!disasmfile->bytecode_file)
    {
      fclose(disasmfile->bytecode_file);
    }


  free(disasmfile->bytecode_buffer);

  disasmfile->bytecode_buffer = nullptr;
  disasmfile->bytecode_file = nullptr;
  disasmfile->bytecode_file_size = 0;
  disasmfile->err_code = 0;

  return EXECUTION_SUCCESS;
}
