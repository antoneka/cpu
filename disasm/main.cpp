#include "disasm.h"

int main()
{
  DisAsmFile disasmfile = {};
  
  disAsmFileCtor(&disasmfile);

  outputDisAsmFile(&disasmfile);

  disAsmFileDtor(&disasmfile);

  return 0;
}
