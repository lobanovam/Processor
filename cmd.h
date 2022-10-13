#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

DEF_CMD(HLT, 0, 0,
{
    printf("in hlt\n");
    return 0;
})

DEF_CMD(PUSH, 1, 1,
{
   PushArg(cpu, stk);
})

DEF_CMD(POP, 2, 1,
{
    PopArg(cpu, stk);
})

DEF_CMD(ADD, 3, 0,
{
    printf("in add\n");
    int a = StackPop(stk);
    int b = StackPop(stk);
    StackPush(stk, a + b);
})

DEF_CMD(SUB, 4, 0,
{
    printf("in sub\n");
    int a = StackPop(stk);
    int b = StackPop(stk);
    StackPush(stk, a - b);
})

DEF_CMD(MUL, 5, 0,
{
    printf("in mul\n");
    int a = StackPop(stk);
    int b = StackPop(stk);
    StackPush(stk, a * b);
})

DEF_CMD(DIV, 6, 0,
{
    printf("in div\n");
    int a = StackPop(stk);
    int b = StackPop(stk);
    if (b == 0) {
        printf("ERROR: can't divide by zero\n");
        return DEV_BY_0;
    } else {
    StackPush(stk, a * b);
    }

})

DEF_CMD(IN, 7, 0,
{
    printf("in in\n");
    printf("enter a number");
    int value = 0;

    while (!scanf("%d", &value)) {
        printf("please, enter a number\n");
        clearBuff();
    }
    clearBuff();
    StackPush(stk, value);

})

DEF_CMD(OUT, 8, 0,
{
    printf("in out\n");
    int a = StackPop(stk);
    printf("out print is %d\n", a);
})

DEF_CMD(JUMP, 9, 1,
{
    cpu->ip = cpu->code[cpu->ip+1] - 1;
})


#endif // CMD_H_INCLUDED
