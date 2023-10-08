#include "assembler.h"

//#########################################################################################################

static int initBuffer(AsmFile *asmfile);

static void splitBuffer(AsmFile *asmfile);

static int initWordArray(AsmFile *asmfile);

static int initCommandArray(AsmFile *asmfile);

static int identifyCommand(char *cmd);

static int identifyParam(Command *cmd, char *param);

//#########################################################################################################

int asmFileCtor(AsmFile *asmfile)
{
  assert(asmfile != nullptr);

  asmfile->cmd_file = fopen("cmd_file.txt", "r");

  if (asmfile->cmd_file == nullptr)
    {
      return OPEN_CMD_FILE_ERROR;
    }

  asmfile->buffer = nullptr;
  asmfile->word_arr = nullptr;
  asmfile->command_arr = nullptr;
  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->commands_num = 0;
  asmfile->err_code = 0;

  int init_buffer_status = initBuffer(asmfile);

  if (init_buffer_status != EXECUTION_SUCCESS)
    {
      return init_buffer_status;
    }

  int init_word_array = initWordArray(asmfile);

  if (init_word_array != EXECUTION_SUCCESS)
    {
      return init_word_array;
    }

  int init_command_array = initCommandArray(asmfile);

  if (init_command_array != EXECUTION_SUCCESS)
    {
      return init_command_array;
    }

  return EXECUTION_SUCCESS;
}

//#########################################################################################################

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

//#########################################################################################################

static void splitBuffer(AsmFile *asmfile)
{
  if (isspace(asmfile->buffer[0]))
    {
      asmfile->buffer[0] = '\0';
    }
  
  for (size_t symb_cnt = 1; symb_cnt < asmfile->cmd_file_size; symb_cnt++) 
    {
      if (isspace(asmfile->buffer[symb_cnt]) && asmfile->buffer[symb_cnt - 1] == '\0')
        {
          asmfile->buffer[symb_cnt] = '\0';
          continue;
        }

      else if (isspace(asmfile->buffer[symb_cnt]))
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

//########################################################################################################

static int initWordArray(AsmFile *asmfile)
{
  asmfile->word_arr = (char**)calloc(asmfile->words_num, sizeof(char*));

  if (asmfile->word_arr == nullptr)
    {
      asmfile->err_code |= WORD_ARR_ALLOCATION_ERROR;

      return WORD_ARR_ALLOCATION_ERROR;
    }

  size_t symb_cnt = 0;

  while(asmfile->buffer[symb_cnt] == '\0')
    {
      symb_cnt++;
    }

  asmfile->word_arr[0] = asmfile->buffer + symb_cnt;
  
  for (size_t words_cnt = 1; words_cnt < asmfile->words_num; symb_cnt++)
    {
      if (asmfile->buffer[symb_cnt] == '\0' && asmfile->buffer[symb_cnt + 1] != '\0')
        {
          asmfile->word_arr[words_cnt++] = asmfile->buffer + symb_cnt + 1;
        }
    }

  return EXECUTION_SUCCESS;
}

//########################################################################################################

static int initCommandArray(AsmFile *asmfile)
{
  asmfile->command_arr = (Command*)calloc(asmfile->commands_num, sizeof(Command));

  if (asmfile->command_arr == nullptr)
    {
      asmfile->err_code |= COMMAND_ARR_ALLOCATION_ERROR;

      return COMMAND_ARR_ALLOCATION_ERROR;
    }

  for (size_t commands_cnt = 0, words_cnt = 0; commands_cnt < asmfile->commands_num; commands_cnt++)
    {
      asmfile->command_arr[commands_cnt].code = identifyCommand(asmfile->word_arr[words_cnt++]);

      if (asmfile->command_arr[commands_cnt].code == INVALID_COMMAND_ERROR)
        {
          asmfile->err_code |= INVALID_COMMAND_ERROR;
          
          return INVALID_COMMAND_ERROR;
        }


      asmfile->command_arr[commands_cnt].param = identifyParam(&asmfile->command_arr[commands_cnt], 
                                                                asmfile->word_arr[words_cnt]);

      if (asmfile->command_arr[commands_cnt].param == INVALID_PARAM_ERROR)
        {
          asmfile->err_code |= INVALID_PARAM_ERROR;

          return INVALID_PARAM_ERROR;
        }

      if (CHECK_PARAM_TYPE(asmfile->command_arr[commands_cnt].code) != NO_PARAM_TYPE)
        {
          words_cnt++;
        }
    }

  return EXECUTION_SUCCESS;
}

//########################################################################################################

static int identifyParam(Command *cmd, char *param)
{
  if (cmd->code != PUSH && cmd->code != POP)
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

//########################################################################################################

static int identifyCommand(char *cmd)
{
  if (strcasecmp(cmd, "push") == 0)   return PUSH;
  if (strcasecmp(cmd, "pop") == 0)    return POP;
  if (strcasecmp(cmd, "hlt") == 0)    return HLT;
  if (strcasecmp(cmd, "in") == 0)     return IN;
  if (strcasecmp(cmd, "out") == 0)    return OUT;
  if (strcasecmp(cmd, "add") == 0)    return ADD;
  if (strcasecmp(cmd, "sub") == 0)    return SUB;
  if (strcasecmp(cmd, "mul") == 0)    return MUL;
  if (strcasecmp(cmd, "div") == 0)    return DIV;
  if (strcasecmp(cmd, "sqrt") == 0)   return SQRT;
  if (strcasecmp(cmd, "sin") == 0)    return SIN;
  if (strcasecmp(cmd, "cos") == 0)    return COS;
  
  return INVALID_COMMAND_ERROR;
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

  for (size_t commands_cnt = 0; commands_cnt < asmfile->commands_num; commands_cnt++)
    {
      if (CHECK_PARAM_TYPE(asmfile->command_arr[commands_cnt].code) == NO_PARAM_TYPE)
        {
          fprintf(asmout, "%d\n", asmfile->command_arr[commands_cnt].code);
        }
      else
        {
          fprintf(asmout, "%d %d\n", asmfile->command_arr[commands_cnt].code,
                                     asmfile->command_arr[commands_cnt].param);
        }
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
  asmfile->commands_num = 0;
  asmfile->err_code = 0;

  free(asmfile->buffer);
  free(asmfile->word_arr);
  free(asmfile->command_arr);

  return EXECUTION_SUCCESS;
}
