#include <string.h>
#include <sys\stat.h>
#include <ctype.h>

#include "cmd.h"
#include "stack.cpp"

const char* CPU_SIGNATURE = "CP";

enum CPU_ERRORS{
    WRONG_SIGNATURE = 1,
    UNDEFINED_CMD,
    DEV_BY_0
};

void clearBuff();

int main() {

    FILE* a_code = NULL;

    if ((a_code = fopen(CODE, "r")) == NULL) {
        printf("can't open a_code file\n");
    }
    ASSERT(a_code != NULL);

    printf("opened\n");

    struct Stack stk1;
    STACK_CTOR(stk1, default_size);

    printf("stk1 created\n");

    size_t cmd_ct = 0;
    char a_sign[20] = "";

    fscanf(a_code, "%s", a_sign);
    fscanf(a_code, "%zd", &cmd_ct);

    if (strcmp(a_sign, CPU_SIGNATURE)) {
        printf("ERROR: assembler and cpu are not compatible\n");
        return WRONG_SIGNATURE;
    }

    printf("sign is %s\n", a_sign);
    printf("cmd_ct is %zd\n", cmd_ct);

    int* code = (int*)(calloc(cmd_ct, sizeof(int)));

    for (int i = 0; i < cmd_ct; i++) {
        fscanf(a_code, "%d", &code[i]);
        printf("%d ", code[i]);
    }
    printf("\n");

    size_t ip = 0;
    int value = 0;
    int value1 = 0;


    while (code[ip] != CMD_HLT) {
        switch (code[ip]) {

            case CMD_PUSH:
                StackPush(&stk1, code[ip+1]);
                ip+=2;
                break;

            case CMD_ADD:
                StackPush(&stk1, StackPop(&stk1) + StackPop(&stk1));
                ip++;
                break;

            case CMD_SUB:
                StackPush(&stk1, StackPop(&stk1) - StackPop(&stk1));
                ip++;
                break;

            case CMD_MUL:
                StackPush(&stk1, StackPop(&stk1) * StackPop(&stk1));
                ip++;
                break;

            case CMD_DIV:
                value = StackPop(&stk1);
                value1 = StackPop(&stk1);
                if (value1){
                    StackPush(&stk1, value / value1);
                }
                else {
                    printf("ERROR: can't divide by 0, (instruction pointer %zd)\n", ip);
                    return DEV_BY_0;
                }
                ip++;
                break;

            case CMD_IN:

                printf("enter a number");
                while (!scanf("%d", &value)) {
                    printf("please, enter a number\n");
                    clearBuff();
                }
                clearBuff();
                StackPush(&stk1, value);
                ip++;
                break;

            case CMD_OUT:
                printf("%d\n", StackPop(&stk1));
                ip++;
                break;

            default:
                printf("ERROR: undefined \"%zd\" instruction \n", code[ip]);
                return UNDEFINED_CMD;
        }

    }
    printf("ok, im done here");

    return 0;
}

void clearBuff()
{
    char ch = 0;
    while((ch = getchar()) != '\n')
        continue;
}
