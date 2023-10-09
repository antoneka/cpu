#ifndef COMMON_H
#define COMMON_H

//#########################################################################################################

#include "stdio.h"

//#########################################################################################################

#define CHECK_CMD(code) ((code) & 0b00001111)
#define CHECK_PARAM_TYPE(code) ((code) & 0b11110000)

//#########################################################################################################

enum ParamTypes
{
  IMMEDIATE_CONST_TYPE          = 1 << 4,
  REGISTER_TYPE                 = 1 << 5,
  NO_PARAM_TYPE                 =      0,
};

enum Commands 
{
  PUSH    =  1,
  POP     =  2,
  HLT     =  3,
  IN      =  4,
  OUT     =  5,
  ADD     =  6, 
  SUB     =  7,
  MUL     =  8,
  DIV     =  9,
  SQRT    = 10,
  SIN     = 11,
  COS     = 12,
};

enum ExecutionStatus
{
  EXECUTION_SUCCESS             =       0,
  BUFFER_ALLOCATION_ERROR       = 1 <<  0,
  WORD_ARR_ALLOCATION_ERROR     = 1 <<  1,
  COMMAND_ARR_ALLOCATION_ERROR  = 1 <<  2,
  OPEN_CMD_FILE_ERROR           = 1 <<  3,
  WRITE_BYTECODE_FILE_ERROR     = 1 <<  4,
  INVALID_COMMAND_ERROR         = 1 << 29,
  INVALID_PARAM_ERROR           = 1 << 30,

  OPEN_BYTECODE_FILE_ERROR      = 1 <<  5,
  WRITE_DISASM_FILE_ERROR       = 1 <<  6,
};

//#########################################################################################################

size_t getFileSize(const char *filename);

#endif // COMMON_H
