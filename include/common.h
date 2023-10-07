#ifndef COMMON_H
#define COMMON_H

#include "stdio.h"

enum Commands 
{
  push    =  1,
  pop     =  2,
  HLT     =  3,
  in      =  4,
  out     =  5,
  add     =  6, 
  sub     =  7,
  mul     =  8,
  divide  =  9,
  sqrt    = 10,
  sin     = 11,
  cos     = 12,
};

//=========================================================================================================

size_t getFileSize(const char *filename);

#endif // COMMON_H
