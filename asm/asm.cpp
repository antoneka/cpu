#include "asm.h"

//#########################################################################################################

static int initCommandBuffer(AsmFile *asmfile);

static void splitCommandBuffer(AsmFile *asmfile);

static int initWordArray(AsmFile *asmfile);

static int initCommandArray(AsmFile *asmfile);

static int getParam(char *param, Command *cmd);

//#########################################################################################################

int asmFileCtor(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  asmfile->cmd_file = fopen("cmd_file.txt", "r");

  if (asmfile->cmd_file == nullptr) 
    { 
      return OPEN_CMD_FILE_ERROR; 
    }

  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->cmds_num = 0;
  asmfile->err_code = 0;

  int init_buffer_status = initCommandBuffer(asmfile);

  if (init_buffer_status != EXECUTION_SUCCESS)
    {
      return init_buffer_status;
    }

  int init_word_arr_status = initWordArray(asmfile);

  if (init_word_arr_status != EXECUTION_SUCCESS)
    {
      return init_word_arr_status;
    }

  int init_cmd_arr_status = initCommandArray(asmfile);

  if (init_cmd_arr_status != EXECUTION_SUCCESS)
    {
      return init_cmd_arr_status;
    }

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

static int initCommandBuffer(AsmFile *asmfile)
{
  asmfile->cmd_file_size = getFileSize(asmfile->cmd_file);

  asmfile->cmd_buff = (char*)calloc(asmfile->cmd_file_size, sizeof(char));

  if (asmfile->cmd_buff == nullptr)
    {
      asmfile->err_code |= BUFFER_ALLOCATION_ERROR;

      return BUFFER_ALLOCATION_ERROR;
    }

  fread(asmfile->cmd_buff, sizeof(char), asmfile->cmd_file_size, asmfile->cmd_file);

  splitCommandBuffer(asmfile);

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

static void splitCommandBuffer(AsmFile *asmfile)
{
  char *cmd_buffer = asmfile->cmd_buff;

  if (isspace(cmd_buffer[0]))
    {
      cmd_buffer[0] = '\0';
    }
  
  for (size_t symb_cnt = 1; symb_cnt < asmfile->cmd_file_size; symb_cnt++) 
    {
      if (isspace(cmd_buffer[symb_cnt]) && cmd_buffer[symb_cnt - 1] == '\0')
        {
          cmd_buffer[symb_cnt] = '\0';

          continue;
        }

      else if (isspace(cmd_buffer[symb_cnt]))
        {
          asmfile->words_num++;

          if (cmd_buffer[symb_cnt] == '\n')
            {
              asmfile->cmds_num++; 
            }

          cmd_buffer[symb_cnt] = '\0';
        } 
    }
}

//########################################################################################################

static int initWordArray(AsmFile *asmfile)
{
  asmfile->word_arr = (char**)calloc(asmfile->words_num, sizeof(char*));

  if (asmfile->word_arr == nullptr)
    {
      asmfile->err_code |= WORD_ARR_ALLOCATION_ERROR;

      return WORD_ARR_ALLOCATION_ERROR;
    }

  char *cmd_buffer = asmfile->cmd_buff;
  char **word_arr = asmfile->word_arr;
  size_t symb_cnt = 0;

  for(; cmd_buffer[symb_cnt] == '\0'; symb_cnt++) {}

  word_arr[0] = cmd_buffer + symb_cnt;
  
  for (size_t word_cnt = 1; word_cnt < asmfile->words_num; symb_cnt++)
    {
      if (cmd_buffer[symb_cnt] == '\0' && cmd_buffer[symb_cnt + 1] != '\0')
        {
          word_arr[word_cnt++] = cmd_buffer + symb_cnt + 1;
        }
    }

  return EXECUTION_SUCCESS;
}

//########################################################################################################

#define DEF_CMD(cmd, params, ...)                                   \
      if (!strcasecmp(#cmd, word_arr[word_cnt]))                    \
        {                                                           \
          cmd_arr[cmd_cnt].code = cmd;                              \
                                                                    \
          if (params)                                               \
            {                                                       \
              word_cnt++;                                           \
                                                                    \
              int get_param_status = getParam(word_arr[word_cnt],   \
                                              &cmd_arr[cmd_cnt]);   \
                                                                    \
              if (get_param_status != EXECUTION_SUCCESS)            \
                {                                                   \
                  asmfile->err_code |= get_param_status;            \
                                                                    \
                  return get_param_status;                          \
                }                                                   \
            }                                                       \
                                                                    \
          word_cnt++;                                               \
        }                                                           \
      else

static int initCommandArray(AsmFile *asmfile)
{
  asmfile->cmd_arr = (Command*)calloc(asmfile->cmds_num, sizeof(Command));

  if (asmfile->cmd_arr == nullptr)
    {
      asmfile->err_code |= COMMAND_ARR_ALLOCATION_ERROR;

      return COMMAND_ARR_ALLOCATION_ERROR;
    }

  Command *cmd_arr = asmfile->cmd_arr;
  char **word_arr = asmfile->word_arr;

  for (size_t cmd_cnt = 0, word_cnt = 0; cmd_cnt < asmfile->cmds_num; cmd_cnt++)
    {
      #include "../include/commands.h"

        {
          asmfile->err_code |= INVALID_COMMAND_ERROR;

          return INVALID_COMMAND_ERROR;
        }
    }

  return EXECUTION_SUCCESS;
}

#undef DEF_CMD

//########################################################################################################

static int getParam(char *param, Command *cmd)
{
  if (isdigit(param[0]))
    {
      for (size_t i = 1; i < strlen(param); i++)
        {
          if (!isdigit(param[i])) 
            { 
              return INVALID_PARAM_ERROR; 
            }
        }

      cmd->code |= IMMEDIATE_CONST_TYPE;
      cmd->param = atoi(param);
    }

  else if (strlen(param) == 3 && param[0] == 'r' && param[2] == 'x')
    {
      if (param[1] < 'a' || 'c' < param[1]) 
        { 
          return INVALID_PARAM_ERROR; 
        }

      cmd->code |= REGISTER_TYPE;
      cmd->param = param[1] - 'a';
    }

  else 
    {
      return INVALID_PARAM_ERROR;
    }

  return EXECUTION_SUCCESS;
}

//########################################################################################################

int outputByteCodeFile(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  FILE *bytecode_file = fopen("bytecode_file.txt", "w");

  if (bytecode_file == nullptr)
    {
      asmfile->err_code |= WRITE_BYTECODE_FILE_ERROR;

      return WRITE_BYTECODE_FILE_ERROR;
    }

  Command *cmd_arr = asmfile->cmd_arr;

  for (size_t cmd_cnt = 0; cmd_cnt < asmfile->cmds_num; cmd_cnt++)
    {
      fprintf(bytecode_file, "%d", cmd_arr[cmd_cnt].code);

      if (GET_PARAM_TYPE(cmd_arr[cmd_cnt].code) != NO_PARAM_TYPE)
        {
          fprintf(bytecode_file, " %d", cmd_arr[cmd_cnt].param);
        }

      fprintf(bytecode_file, "\n");
    }

  fclose(bytecode_file);

  return EXECUTION_SUCCESS;
}

//########################################################################################################

int outputBinFile(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  FILE *bin_file = fopen("asm_file.bin", "wb");

  if (bin_file == nullptr)
    {
      asmfile->err_code |= WRITE_BIN_FILE_ERROR;

      return WRITE_BIN_FILE_ERROR;
    }

  int *binary_code_arr = (int*)calloc(asmfile->words_num, sizeof(int));

  if (binary_code_arr == nullptr)
    {
      asmfile->err_code |= BINARY_ARR_ALLOCATION_ERROR;

      return BINARY_ARR_ALLOCATION_ERROR;
    }

  Command *cmd_arr = asmfile->cmd_arr;

  for (size_t token_cnt = 0, cmd_cnt = 0; token_cnt < asmfile->words_num; cmd_cnt++)
    {
      binary_code_arr[token_cnt++] = cmd_arr[cmd_cnt].code;

      if (GET_PARAM_TYPE(cmd_arr[cmd_cnt].code) != NO_PARAM_TYPE)
        {
          binary_code_arr[token_cnt++] = cmd_arr[cmd_cnt].param;
        }
    }

  fwrite(binary_code_arr, sizeof(int), asmfile->words_num, bin_file);

  free(binary_code_arr);
  fclose(bin_file);

  return EXECUTION_SUCCESS;
}

//########################################################################################################

int asmFileDtor(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  if (asmfile->cmd_file != nullptr) 
    {
      fclose(asmfile->cmd_file);
      asmfile->cmd_file = nullptr;
    }

  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->cmds_num = 0;
  asmfile->err_code = 0;

  free(asmfile->cmd_buff);
  asmfile->cmd_buff = nullptr;

  free(asmfile->word_arr);
  asmfile->word_arr = nullptr;

  free(asmfile->cmd_arr);
  asmfile->cmd_arr = nullptr;

  return EXECUTION_SUCCESS;
}
