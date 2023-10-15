#ifndef COMMON_H
#define COMMON_H

//#########################################################################################################

#include "stdio.h"

//#########################################################################################################

#define GET_CMD(code) ((code) & 0b00001111)
#define GET_PARAM_TYPE(code) ((code) & 0b11110000)

//#########################################################################################################

enum ParamTypes
{
  IMMEDIATE_CONST_TYPE          = 1 << 4,
  REGISTER_TYPE                 = 1 << 5,
  NO_PARAM_TYPE                 =      0,
};

enum Commands 
{
  HLT     =  0,
  PUSH    =  1,
  POP     =  2,
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

enum ExecStatus
{
  // assembler 
  EXECUTION_SUCCESS             =       0,
  BUFFER_ALLOCATION_ERROR       = 1 <<  0,
  WORD_ARR_ALLOCATION_ERROR     = 1 <<  1,
  COMMAND_ARR_ALLOCATION_ERROR  = 1 <<  2,
  OPEN_CMD_FILE_ERROR           = 1 <<  3,
  WRITE_BYTECODE_FILE_ERROR     = 1 <<  4,
  INVALID_COMMAND_ERROR         = 1 << 29,
  INVALID_PARAM_ERROR           = 1 << 30,
  BINARY_ARR_ALLOCATION_ERROR   = 1 << 20,
  WRITE_BIN_FILE_ERROR          = 1 << 21,

  // disassembler
  OPEN_BYTECODE_FILE_ERROR      = 1 <<  5,
  WRITE_DISASM_FILE_ERROR       = 1 <<  6,

  // stack
  STACK_ALLOCATION_ERROR        = 1 <<  7,
  STACK_CAPACITY_ERROR          = 1 <<  8,
  STACK_SIZE_ERROR              = 1 <<  9,
  LEFT_CANARY_STACK_CORRUPTED   = 1 << 10,
  RIGHT_CANARY_STACK_CORRUPTED  = 1 << 11,
  LEFT_CANARY_DATA_CORRUPTED    = 1 << 12,
  RIGHT_CANARY_DATA_CORRUPTED   = 1 << 13,
  STACK_REALLOCATION_ERROR      = 1 << 14,
  STACK_ALREADY_CONSTRUCTED     = 1 << 15,
  STACK_ALREADY_DESTRUCTED      = 1 << 16,
  STACK_PTR_IS_NULL             = 1 << 17,
  POPVALUE_PTR_IS_NULL          = 1 << 18,
  HASH_CORRUPTED                = 1 << 19,
};

//#########################################################################################################

size_t getFileSize(const char *filename);

#endif // COMMON_H
