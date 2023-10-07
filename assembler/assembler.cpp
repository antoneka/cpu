#include "assembler.h"

//=========================================================================================================

static int initBuffer(AsmFile *asmfile);

static void splitBuffer(AsmFile *asmfile);

static int initWordsArray(AsmFile *asmfile);

static int initCommandsArray(AsmFile *asmfile);

static int identifyCommand(char *cmd);

static int identifyParam(Command *cmd, char *param);

//=========================================================================================================

int asmFileCtor(AsmFile *asmfile)
{
  asmfile->cmd_file = fopen("cmd_file.txt", "r");

  if (asmfile->cmd_file == nullptr)
    {
      return OPEN_CMD_FILE_ERROR;
    }

  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->commands_num = 0;
  asmfile->err_code = 0;

  int init_buffer_status = initBuffer(asmfile);

  if (init_buffer_status != EXECUTION_SUCCESS)
    {
      return init_buffer_status;
    }

  int init_words_array = initWordsArray(asmfile);

  if (init_words_array != EXECUTION_SUCCESS)
    {
      return init_words_array;
    }

  int init_commands_array = initCommandsArray(asmfile);

  if (init_commands_array != EXECUTION_SUCCESS)
    {
      return init_commands_array;
    }

  return EXECUTION_SUCCESS;
}

static int initBuffer(AsmFile *asmfile)
{
  asmfile->cmd_file_size = getFileSize("cmd_file.txt");

  asmfile->buffer = (char*)calloc(asmfile->cmd_file_size, sizeof(char));

  if (asmfile->buffer == nullptr)
    {
      asmfile->err_code |= BUFFER_ALLOCATION_ERROR;

      return BUFFER_ALLOCATION_ERROR;
    }

  fread(asmfile->buffer, sizeof(char), asmfile->cmd_file_size, asmfile->cmd_file);

  splitBuffer(asmfile);

  return EXECUTION_SUCCESS;
}

static void splitBuffer(AsmFile *asmfile)
{
  for (size_t symb_cnt = 0; symb_cnt < asmfile->cmd_file_size; symb_cnt++) 
    {
      if (isspace(asmfile->buffer[symb_cnt]))
        {
          asmfile->words_num++;

          if (asmfile->buffer[symb_cnt] == '\n')
            {
              asmfile->commands_num++; 
            }

          asmfile->buffer[symb_cnt] = '\0';
        } 
    }
}

static int initWordsArray(AsmFile *asmfile)
{
  asmfile->words_arr = (char**)calloc(asmfile->words_num, sizeof(char*));

  if (asmfile->words_arr == nullptr)
    {
      asmfile->err_code |= WORDS_ARR_ALLOCATION_ERROR;

      return WORDS_ARR_ALLOCATION_ERROR;
    }

  asmfile->words_arr[0] = asmfile->buffer;
  
  for (size_t symb_cnt = 0, words_cnt = 1; words_cnt < asmfile->words_num; symb_cnt++)
    {
      if (asmfile->buffer[symb_cnt] == '\0')
        {
          asmfile->words_arr[words_cnt++] = asmfile->buffer + symb_cnt + 1;
        }
    }

  return EXECUTION_SUCCESS;
}

static int initCommandsArray(AsmFile *asmfile)
{
  // sscanf("%d") работает для char !!
  asmfile->commands_arr = (Command*)calloc(asmfile->commands_num, sizeof(Command));

  if (asmfile->commands_arr == nullptr)
    {
      asmfile->err_code |= COMMANDS_ARR_ALLOCATION_ERROR;

      return COMMANDS_ARR_ALLOCATION_ERROR;
    }

  for (size_t commands_cnt = 0, words_cnt = 0; commands_cnt < asmfile->commands_num; commands_cnt++)
    {
      /*
      if (isspace(asmfile->words_arr[words_cnt][0]))
        {
          words_cnt++;

          continue;
        }
      */

      asmfile->commands_arr[commands_cnt].code = identifyCommand(asmfile->words_arr[words_cnt++]);

      if (asmfile->commands_arr[commands_cnt].code == INVALID_COMMAND_ERROR)
        {
          asmfile->err_code |= INVALID_COMMAND_ERROR;
          
          return INVALID_COMMAND_ERROR;
        }


      asmfile->commands_arr[commands_cnt].param = identifyParam(&asmfile->commands_arr[commands_cnt], 
                                                                asmfile->words_arr[words_cnt]);

      if (asmfile->commands_arr[commands_cnt].param == INVALID_PARAM_ERROR)
        {
          asmfile->err_code |= INVALID_PARAM_ERROR;

          return INVALID_PARAM_ERROR;
        }

      if (CHECK_PARAM_TYPE(asmfile->commands_arr[commands_cnt].code) != NO_PARAM_TYPE)
        {
          words_cnt++;
        }
    }

  return EXECUTION_SUCCESS;
}

static int identifyParam(Command *cmd, char *param)
{
  if (cmd->code != push && cmd->code != pop)
    {
      cmd->code |= NO_PARAM_TYPE;

      return 0;
    } 

  else if (isdigit(param[0]))
    {
      for (size_t i = 1; i < strlen(param); i++)
        {
          if (!isdigit(param[i]))
            {
              return INVALID_PARAM_ERROR;
            }
        }

      cmd->code |= IMMEDIATE_CONST_TYPE;

      return atoi(param);
    }

  else if (strlen(param) == 3 && param[0] == 'r' && param[2] == 'x')
    {
      if (param[1] < 'a' || 'c' < param[1])
        {
          return INVALID_PARAM_ERROR;
        }

      cmd->code |= REGISTER_TYPE;

      return param[1] - 'a';
    }

  return INVALID_PARAM_ERROR;
}

static int identifyCommand(char *cmd)
{
  if (strcmp(cmd, "push") == 0) 
    {
      return push;
    }
  else if (strcmp(cmd, "pop") == 0) 
    {
      return pop;
    }
  else if (strcmp(cmd, "HLT") == 0) 
    {
      return HLT;
    }
  else if (strcmp(cmd, "in") == 0) 
    {
      return in;
    }
  else if (strcmp(cmd, "out") == 0) 
    {
      return out;
    }
  else if (strcmp(cmd, "out") == 0) 
    {
      return out;
    }
  else if (strcmp(cmd, "add") == 0) 
    {
      return add;
    }
  else if (strcmp(cmd, "sub") == 0) 
    {
      return sub;
    }
  else if (strcmp(cmd, "mul") == 0) 
    {
      return mul;
    }
  else if (strcmp(cmd, "divide") == 0) 
    {
      return divide;
    }
  else if (strcmp(cmd, "sqrt") == 0) 
    {
      return sqrt;
    }
  
  return INVALID_COMMAND_ERROR;
}

int outputAsmFile(AsmFile *asmfile)
{
  FILE *asmout = fopen("bytecode_file.txt", "w");

  if (asmout == nullptr)
    {
      asmfile->err_code |= WRITE_BYTE_CODE_FILE_ERROR;

      return WRITE_BYTE_CODE_FILE_ERROR;
    }

  for (size_t commands_cnt = 0; commands_cnt < asmfile->commands_num; commands_cnt++)
    {
      if (CHECK_PARAM_TYPE(asmfile->commands_arr[commands_cnt].code) == NO_PARAM_TYPE)
        {
          fprintf(asmout, "%d\n", CHECK_CMD(asmfile->commands_arr[commands_cnt].code));
        }
      else
        {
          fprintf(asmout, "%d %d\n", CHECK_CMD(asmfile->commands_arr[commands_cnt].code),
                                     asmfile->commands_arr[commands_cnt].param);
        }
    }

  fclose(asmout);

  return EXECUTION_SUCCESS;
}

int asmFileDtor(AsmFile *asmfile)
{
  if (asmfile->cmd_file != nullptr) 
    {
      fclose(asmfile->cmd_file);
    }

  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->commands_num = 0;
  asmfile->err_code = 0;

  free(asmfile->buffer);
  free(asmfile->words_arr);
  free(asmfile->commands_arr);

  return EXECUTION_SUCCESS;
}
