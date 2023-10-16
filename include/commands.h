#define DO_PUSH(elem) stackPush(stk, elem)
#define DO_POP(var)   stackPop(stk, var)
#define REGS          cpu->regs

DEF_CMD(HLT, 0, 
  {
    return 0;
  })

DEF_CMD(PUSH, 1, 
  {
    DO_PUSH(*param_ptr);
  })

DEF_CMD(POP, 1, 
  {
    DO_POP(param_ptr);
  })
    
DEF_CMD(IN, 0, 
  {
    elem_t elem = 0;
    scanf("" ELEM_MODIFIER "", &elem);

    DO_PUSH(elem);
  })

DEF_CMD(OUT, 0, 
  {
    printf("CPU OUT:\n");
    STACK_DUMP(stk, stdout);
  })

DEF_CMD(ADD, 0, 
  {
     elem_t a = 0;
     elem_t b = 0;

     DO_POP(&a); 
     DO_POP(&b);

     DO_PUSH(a + b);
  })

DEF_CMD(SUB, 0, 
  {
     elem_t a = 0;
     elem_t b = 0;

     DO_POP(&a); 
     DO_POP(&b);

     DO_PUSH(b - a);
  })

DEF_CMD(MUL, 0, 
  {
     elem_t a = 0;
     elem_t b = 0;

     DO_POP(&a); 
     DO_POP(&b);

     DO_PUSH(a * b);
  })

DEF_CMD(DIV, 0, 
  {
     elem_t a = 0;
     elem_t b = 0;

     DO_POP(&a); 
     DO_POP(&b);

     DO_PUSH(b / a);
  })

DEF_CMD(SQRT, 0, 
  {
     elem_t num = 0;

     DO_POP(&num); 

     DO_PUSH(sqrt(num));
  })

DEF_CMD(SIN, 0, 
  {
     elem_t num = 0;

     DO_POP(&num); 

     DO_PUSH(sin(num));
  })

DEF_CMD(COS, 0, 
  {
     elem_t num = 0;

     DO_POP(&num); 

     DO_PUSH(cos(num));
  })
