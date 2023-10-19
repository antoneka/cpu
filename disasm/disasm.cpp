#include "disasm.h"
#include "../include/common.h"

//----------------------------------------------------------------------------------------------------------

int disAsmFileCtor(DisAsmFile *disasmfile)
{
  assert(disasmfile != nullptr);

  disasmfile->bin_file = fopen("asm_file.bin", "rb");

  if (disasmfile->bin_file == nullptr)
    {
      return OPEN_BYTECODE_FILE_ERROR;
    }

  disasmfile->bin_file_size = getFileSize(disasmfile->bin_file);
  disasmfile->err_code = 0;

  int init_buffer_status = initAsmBuffer(&disasmfile->buffer, &disasmfile->buff_size, 
                                         disasmfile->bin_file, disasmfile->bin_file_size);

  if (init_buffer_status != EXECUTION_SUCCESS)
    {
      return init_buffer_status;
    }

  return EXECUTION_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------

#define DEF_CMD(cmd, params, ...)                                       \
          case cmd:                                                     \
            {                                                           \
              fprintf(output_disasm, #cmd);       \
                                                                        \
              if (params) \
                { \
                  if (GET_PARAM_TYPE(read_cmd) == IMMEDIATE_CONST_TYPE) \
                    { \
                      fprintf(output_disasm, " %d", buffer[++cmd_cnt]); \
                    }   \
                  else if (GET_PARAM_TYPE(read_cmd) == REGISTER_TYPE) \
                    {   \
                      fprintf(output_disasm, " r%cx", buffer[++cmd_cnt] + 'a'); \
                    }  \
                } \
                                  \
              fprintf(output_disasm, "\n"); \
                    \
              break; \
            }                            

int outputDisAsmFile(DisAsmFile *disasmfile)
{
  FILE *output_disasm = fopen("disasm_file.txt", "w");

  if (output_disasm == nullptr)
    {
      disasmfile->err_code |= WRITE_DISASM_FILE_ERROR;
    }

  int *buffer = disasmfile->buffer;

  for (size_t cmd_cnt = 0; cmd_cnt < disasmfile->buff_size;)
    {
      int read_cmd = buffer[cmd_cnt];

      switch (GET_CMD(read_cmd))  
        {
          #include "../include/commands.h"

          default:
            {
              disasmfile->err_code = INVALID_COMMAND_ERROR;

              return INVALID_COMMAND_ERROR;
            }
        }

      cmd_cnt++;
    }

  fclose(output_disasm);

  return EXECUTION_SUCCESS;
}

#undef DEF_CMD

//----------------------------------------------------------------------------------------------------------

int disAsmFileDtor(DisAsmFile *disasmfile)
{
  if (!disasmfile->bin_file)
    {
      fclose(disasmfile->bin_file);
      disasmfile->bin_file = nullptr;
    }

  free(disasmfile->buffer);

  disasmfile->buffer = nullptr;
  disasmfile->bin_file = nullptr;
  disasmfile->bin_file_size = 0;
  disasmfile->err_code = 0;

  return EXECUTION_SUCCESS;
}
