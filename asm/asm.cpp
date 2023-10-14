#include "asm.h"

//#########################################################################################################

static int initCommandBuffer(AsmFile *asmfile);

static void splitCommandBuffer(AsmFile *asmfile);

static int initWordArray(AsmFile *asmfile);

static int initCommandArray(AsmFile *asmfile);

static int getParam(char *word, Command *cmd);

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

  int init_word_array = initWordArray(asmfile);

  if (init_word_array != EXECUTION_SUCCESS)
    {
      return init_word_array;
    }

  int init_cmd_array = initCommandArray(asmfile);

  if (init_cmd_array != EXECUTION_SUCCESS)
    {
      return init_cmd_array;
    }

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

static int initCommandBuffer(AsmFile *asmfile)
{
  asmfile->cmd_file_size = getFileSize("cmd_file.txt");

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
  
  for (size_t words_cnt = 1; words_cnt < asmfile->words_num; symb_cnt++)
    {
      if (cmd_buffer[symb_cnt] == '\0' && cmd_buffer[symb_cnt + 1] != '\0')
        {
          word_arr[words_cnt++] = cmd_buffer + symb_cnt + 1;
        }
    }

  return EXECUTION_SUCCESS;
}

//########################################################################################################

#define DEF_CMD(cmd, args, ...)                                     \
      if (!strcasecmp(#cmd, word_arr[words_cnt]))                   \
        {                                                           \
          cmd_arr[cmd_cnt].code = cmd;                              \
                                                                    \
          if (args)                                                 \
            {                                                       \
              words_cnt++;                                          \
                                                                    \
              int err_status = getParam(word_arr[words_cnt],        \
                                        &cmd_arr[cmd_cnt]);         \
                                                                    \
              if (err_status != EXECUTION_SUCCESS)                  \
                {                                                   \
                  asmfile->err_code |= err_status;                  \
                                                                    \
                  return err_status;                                \
                }                                                   \
            }                                                       \
                                                                    \
          words_cnt++;                                              \
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

  for (size_t cmd_cnt = 0, words_cnt = 0; cmd_cnt < asmfile->cmds_num; cmd_cnt++)
    {
      #include "../include/commands.h"

        {
          asmfile->err_code |= INVALID_COMMAND_ERROR;

          return INVALID_PARAM_ERROR;
        }
    }

  return EXECUTION_SUCCESS;
}

#undef DEF_CMD

//########################################################################################################

static int getParam(char *word, Command *cmd)
{
  if (isdigit(word[0]))
    {
      for (size_t i = 1; i < strlen(word); i++)
        {
          if (!isdigit(word[i])) 
            { 
              return INVALID_PARAM_ERROR; 
            }
        }

      cmd->code |= IMMEDIATE_CONST_TYPE;
      cmd->param = atoi(word);
      
      return EXECUTION_SUCCESS;
    }

  else if (strlen(word) == 3 && word[0] == 'r' && word[2] == 'x')
    {
      if (word[1] < 'a' || 'c' < word[1]) 
        { 
          return INVALID_PARAM_ERROR; 
        }

      cmd->code |= REGISTER_TYPE;
      cmd->param = word[1] - 'a';

      return EXECUTION_SUCCESS;
    }

  return INVALID_PARAM_ERROR;
}

//########################################################################################################

int outputAsmFile(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  FILE *asmout = fopen("bytecode_file.txt", "w");

  if (asmout == nullptr)
    {
      asmfile->err_code |= WRITE_BYTECODE_FILE_ERROR;

      return WRITE_BYTECODE_FILE_ERROR;
    }

  Command *cmd_arr = asmfile->cmd_arr;

  for (size_t cmd_cnt = 0; cmd_cnt < asmfile->cmds_num; cmd_cnt++)
    {
      fprintf(asmout, "%d", cmd_arr[cmd_cnt].code);

      if (GET_PARAM_TYPE(cmd_arr[cmd_cnt].code) != NO_PARAM_TYPE)
        {
          fprintf(asmout, " %d", cmd_arr[cmd_cnt].param);
        }

      fprintf(asmout, "\n");
    }

  fclose(asmout);

  return EXECUTION_SUCCESS;
}

//########################################################################################################

int asmFileDtor(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  if (asmfile->cmd_file != nullptr) 
    {
      fclose(asmfile->cmd_file);
    }

  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->cmds_num = 0;
  asmfile->err_code = 0;

  free(asmfile->cmd_buff);
  free(asmfile->word_arr);
  free(asmfile->cmd_arr);

  return EXECUTION_SUCCESS;
}
