#include <assert.h>
#include "assembler.h"
#include "../include/common.h"
#include <cstdio>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h> 

  static int initBuffer(AsmFile *asmfile);

  static void splitBuffer(AsmFile *asmfile);

  static int initWordsArray(AsmFile *asmfile);

  static int initTokensArray(AsmFile *asmfile);

  static int identifyCmd(char *cmd);

  static int identifyCmdType(int code, char *param);

static int identifyCmdParam(int code, char *param);

//=========================================================================================================

int asmFileCtor(AsmFile *asmfile)
{
  asmfile->cmd_file = fopen("cmd_file.txt", "r");

  asmfile->cmd_file_size = 0;
  asmfile->words_num = 0;
  asmfile->tokens_num = 0;
  asmfile->err_code = 0;

  int init_buffer_status = initBuffer(asmfile);

  if (init_buffer_status == -1)
    {
      return -1;
    }

  int init_words_array = initWordsArray(asmfile);

  if (init_words_array == -1)
    {
      return -1;
    }

  int init_tokens_array = initTokensArray(asmfile);

  if (init_tokens_array == -1)
    {
      return -1;
    }

  return 0;
}

size_t getFileSize(const char *filename)
{
    assert(filename != nullptr);

    struct stat file_info = {};

    stat(filename, &file_info);

    if (file_info.st_size <= 0)
    {
        // ??
    }

    return (size_t)file_info.st_size;
}

static int initBuffer(AsmFile *asmfile)
{
  asmfile->cmd_file_size = getFileSize("cmd_file.txt");

  asmfile->buffer = (char*)calloc(asmfile->cmd_file_size, sizeof(char));

  if (asmfile->buffer == nullptr)
    {
      return -1; // ??
    }

  fread(asmfile->buffer, sizeof(char), asmfile->cmd_file_size, asmfile->cmd_file);

  splitBuffer(asmfile);

  return 0;
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
              asmfile->tokens_num++; 
            }

          asmfile->buffer[symb_cnt] = '\0';
        } 
    }
}

static int initWordsArray(AsmFile *asmfile)
{
  asmfile->words_arr = (char**)calloc(asmfile->words_num, sizeof(char*));

  asmfile->words_arr[0] = asmfile->buffer;
  
  for (size_t symb_cnt = 0, words_cnt = 1; words_cnt < asmfile->words_num; symb_cnt++)
    {
      if (asmfile->buffer[symb_cnt] == '\0')
        {
          asmfile->words_arr[words_cnt++] = asmfile->buffer + symb_cnt + 1;
        }
    }

  return 0;
}

static int initTokensArray(AsmFile *asmfile)
{
  // sscanf("%d") работает для char !!
  asmfile->tokens_arr = (Token*)calloc(asmfile->tokens_num, sizeof(Token));

  for (size_t tokens_cnt = 0, words_cnt = 0; tokens_cnt < asmfile->tokens_num;)
    {
      /*
      if (isspace(asmfile->words_arr[words_cnt][0]))
        {
          words_cnt++;

          continue;
        }
      */

      asmfile->tokens_arr[tokens_cnt].code = identifyCmd(asmfile->words_arr[words_cnt++]);

      int cmd_type = identifyCmdType(asmfile->tokens_arr[tokens_cnt].code, asmfile->words_arr[words_cnt]);
      asmfile->tokens_arr[tokens_cnt].code |= cmd_type;

      if (cmd_type != 0)
        {
          asmfile->tokens_arr[tokens_cnt].param = identifyCmdParam(asmfile->tokens_arr[tokens_cnt].code,
                                                                asmfile->words_arr[words_cnt]);

          words_cnt++;
        }

      tokens_cnt++;
//      printf("cmd = %d, type = %d, param = %d\n", asmfile->tokens_arr[tokens_cnt].code)
    }

  return 0;
}

static int identifyCmdType(int code, char *param)
{
  if (code != push && code != pop)
    {
      return 0;
    } 

  if (isdigit(param[0]))
    {
      return IMMEDIATE_CONST_MASK;
    }
  else if (strlen(param) == 3 && param[0] == 'r' && param[2] == 'x')
    {
//      printf("code = %d param = %s\n", code, param);
      return REGISTER_MASK;
    }

  return 0;
}

static int identifyCmdParam(int code, char *param)
{
  if ((code & CMD_TYPE_MASK) == IMMEDIATE_CONST_MASK)
    {
      return atoi(param);
    }
  else if ((code & CMD_TYPE_MASK) == REGISTER_MASK)
    {
      if (strlen(param) == 3 && param[0] == 'r' && param[2] == 'x')
        {
          return param[1] - 'a';
        }
    }

  return 0;
}

static int identifyCmd(char *cmd)
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
  
  return 0;
}

void outputAsmFile(AsmFile *asmfile)
{
  FILE *asmout = fopen("asm_file.txt", "w");

  for (size_t tokens_cnt = 0; tokens_cnt < asmfile->tokens_num; tokens_cnt++)
    {
      if ((asmfile->tokens_arr[tokens_cnt].code & CMD_TYPE_MASK) == 0)
        {
          fprintf(asmout, "%d\n", asmfile->tokens_arr[tokens_cnt].code & CMD_MASK);
        }
      else
        {
          fprintf(asmout, "%d %d\n", asmfile->tokens_arr[tokens_cnt].code & CMD_MASK,
                                     asmfile->tokens_arr[tokens_cnt].param);
        }
    }

  fclose(asmout);
}

int asmFileDtor(AsmFile *asmfile)
{
  if (asmfile->cmd_file != nullptr) 
    {
      fclose(asmfile->cmd_file);
    }

  asmfile->words_num = 0;
  asmfile->tokens_num = 0;
  asmfile->err_code = 0;

  free(asmfile->buffer);
  free(asmfile->words_arr);
  free(asmfile->tokens_arr);

  return 0;
}

