#ifndef COMMON_H
#define COMMON_H

#include "stdio.h"

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

//=========================================================================================================

size_t getFileSize(const char *filename);

#endif // COMMON_H
