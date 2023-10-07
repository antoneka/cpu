#include "assembler.h"

int main()
{
  AsmFile asmfile = {};

  asmFileCtor(&asmfile);

  outputAsmFile(&asmfile);

  asmFileDtor(&asmfile);

  return 0;
}
