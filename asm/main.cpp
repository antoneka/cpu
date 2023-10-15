#include "asm.h"

int main()
{
  AsmFile asmfile = {};

  asmFileCtor(&asmfile);

  outputByteCodeFile(&asmfile);

  outputBinFile(&asmfile);

  asmFileDtor(&asmfile);

  return 0;
}
