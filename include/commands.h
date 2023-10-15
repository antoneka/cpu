#define DO_PUSH(elem) stackPush(stk, elem)
#define DO_POP(var)   stackPop(stk, var)
#define REGS          cpu->regs

DEF_CMD(HLT, 0, 
  {
    return 0;
  })

DEF_CMD(PUSH, 1, 
  {
    DO_PUSH(param);
  })

DEF_CMD(POP, 1, 
  {
    DO_POP(param_ptr)
  })
    
DEF_CMD(IN, 0, 
  {
    elem_t arg = 0;
    scanf(" ELEM_MODIFIER ", &arg);

    DO_PUSH(arg);
  })

DEF_CMD(OUT, 0, 
  {
    printf("CPU OUT:\n");
    STACK_DUMP(stk, stdout);
  })

DEF_CMD(ADD, 0, 
  {

  })

DEF_CMD(SUB, 0, 
  {

  })

DEF_CMD(MUL, 0, 
  {

  })

DEF_CMD(DIV, 0, 
  {

  })

DEF_CMD(SQRT, 0, 
  {

  })

DEF_CMD(SIN, 0, 
  {

  })

DEF_CMD(COS, 0, 
  {

  })
