#include <string.h>
#include <sys\stat.h>
#include <ctype.h>
#include <stdio.h>
#include "stack.cpp"
#include "config.h"

#define DEF_CMD(name, num, arg, ...) \
    case num:                        \
    __VA_ARGS__                      \
    break;
                                                                                                                                                                                    #undef calloc(...)
const char* CPU_SIGNATURE = "CP";
const char* CODE = "a_code.txt";

const size_t RAM_SIZE = 100;
const size_t REG_SIZE = 4;

int Compile(struct CPU* cpu, struct Stack* stk);
void clearBuff();

void PushArg(struct CPU* cpu, struct Stack* stk);
void PopArg(struct CPU* cpu, struct Stack* stk);
void CpuCtor(struct CPU* cpu, size_t cmd_ct);
int checkSign(struct CPU* cpu, FILE* a_code);
int getCode(struct CPU* cpu);

const int CMD_MASK = 31; /*11111*/



struct CPU {
    int* code;
    int* regs;
    int* RAM;
    const char* signature;
    size_t ip;
};

enum CPU_ERRORS{
    WRONG_SIGNATURE = 1,
    UNDEFINED_CMD,
    DEV_BY_0,
    INVALID_REG_INDEX,
    INVALID_MEM_INDEX
};

static char* CpuErrorNames[] {
    "Wrong signature",
    "Undefined cmd",
    "Can't devide by zero",
    "Invalid reg index",
    "Invalid RAM index"
};


int main() {

    struct CPU cpu = {NULL, NULL, NULL, CPU_SIGNATURE, 0};

    struct Stack stk1;
    STACK_CTOR(stk1, default_size);
    printf("stk1 created\n");

    getCode(&cpu);

    if (Compile(&cpu, &stk1)){
        printf("ERROR OCCURED\n");
    }

    printf("ok, im done here");

    return 0;
}

int Compile(struct CPU* cpu, struct Stack* stk) {
    while (true){
        switch(cpu->code[cpu->ip] & CMD_MASK) {
            #include "cmd.h"
        default:
            printf("ERROR: undefined command, ip: %zd\n", cpu->ip);
            return UNDEFINED_CMD;
        }
        cpu->ip++;
    }
    #undef DEF_CMD
    return 0;
}

void clearBuff(){
    char ch = 0;
    while((ch = getchar()) != '\n')
        continue;
}

void PushArg(struct CPU* cpu, struct Stack* stk) {
    printf("in push\n");
    int arg = 0;
    int shift = 0;

    if (cpu->code[cpu->ip] & ARG_REG) {
        printf("its reg\n");
        arg += cpu->regs[cpu->code[cpu->ip+1]];
        shift++;
    }

    if (cpu->code[cpu->ip] & ARG_IMMED){
        printf("its immed\n");
        arg += cpu->code[cpu->ip+1+shift];
        shift++;
    }

    if (cpu->code[cpu->ip] & ARG_RAM) {
        printf("its ram\n");
        arg = cpu->RAM[arg];
    }

    printf("arg is %d\n", arg);
    cpu->ip+=shift;

    StackPush(stk, arg);
    printf("push done\n");
}

void PopArg(struct CPU* cpu, struct Stack* stk) {

    printf("in pop\n");
    int arg = StackPop(stk);
    int shift = 0;
    int index = 0;

    if (cpu->code[cpu->ip] & ARG_REG) {
        index += cpu->code[cpu->ip+1];
        shift++;
    }

    if (cpu->code[cpu->ip] & ARG_IMMED){
        index += cpu->code[cpu->ip+1+shift];
    }

    if (cpu->code[cpu->ip] & ARG_RAM) {
        if (index >= RAM_SIZE) {
            printf("ERROR: invalid index of RAM\n");
            exit(INVALID_MEM_INDEX);
        }
        cpu->RAM[index] = arg;

    } else {
        if (index >= REG_SIZE) {
            printf("ERROR: invalid index of REGS\n");
            exit(INVALID_REG_INDEX);
        }
        cpu->regs[index] = arg;
    }
    cpu->ip += shift;
    printf("pop done\n");

}

void CpuCtor(struct CPU* cpu, size_t cmd_ct) {
    cpu->code = (int*)(calloc(cmd_ct, sizeof(int)));
    ASSERT(cpu->code != nullptr);

    cpu->RAM = (int*)(calloc(RAM_SIZE, sizeof(int)));
    ASSERT(cpu->RAM != nullptr);

    cpu->regs = (int*)(calloc(REG_SIZE, sizeof(int)));
    ASSERT(cpu->regs != nullptr);
}

int checkSign(struct CPU* cpu, FILE* a_code){
    char a_sign[20] = "";
    fscanf(a_code, "%s", a_sign);

    if (strcmp(a_sign, cpu->signature)) {
        return WRONG_SIGNATURE;
    }
    return 0;
}

int getCode(struct CPU* cpu) {
    FILE* a_code = NULL;
    if ((a_code = fopen(CODE, "r")) == NULL) {
        printf("can't open a_code file\n");
    }
    ASSERT(a_code != NULL);

    if (checkSign(cpu, a_code))
        return WRONG_SIGNATURE;

    size_t cmd_ct = 0;
    fscanf(a_code, "%zd", &cmd_ct);

    CpuCtor(cpu, cmd_ct);

    for (int i = 0; i < cmd_ct; i++) {
        fscanf(a_code, "%d", &(cpu->code[i]));
        printf("code[%d] = %d\n",i, cpu->code[i]);
    }

    printf("\n");
    fclose(a_code);

}


