#include "cpu.h"


int main()
{
  CPU cpu = {};
  cpuCtor(&cpu);
  executeCommands(&cpu);
  cpuDtor(&cpu);
  


  return 0;
}
