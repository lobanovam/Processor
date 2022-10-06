#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <ctype.h>

#include "cmd.h"
#include "log.h"

void connectPointers(struct Text* text_ptr);
void reachValue(char** ptr);

struct Text {
    char *buff;
    char **pointers;
    size_t ch_count;
    size_t lines_count;
};

const char* SOURCE = "source.txt";

const int CMD_LEN = 20;
const char* ASM_SIGNATURE = "CP";


int main() {

    struct Text text = {nullptr, nullptr, 0, 0};

    FILE* source = NULL;
    if ((source = fopen(SOURCE, "r")) == NULL) {
        printf("can't open source file\n");
    }

    struct stat file;
    stat(SOURCE, &file);
    size_t buffSize = file.st_size;

    text.buff = (char*)(calloc(buffSize, sizeof(char)));

    text.ch_count = fread(text.buff, sizeof(char), buffSize, source);

    for (int i = 0; i < text.ch_count; i++) {
        if (text.buff[i] == '\n') {
            text.lines_count++;
        }
    }
    printf("lines count is %d\n", text.lines_count);

    text.pointers = (char**)(calloc(text.lines_count, sizeof(char*)));

    connectPointers(&text);

    for (int i = 0; i < text.lines_count; i++) {
        printf("%s\n", text.pointers[i]);
    }

    int *code = (int*)(calloc(text.lines_count*2, sizeof(int)));

    size_t line = 0;
    size_t ip = 0;

    while (line < text.lines_count) {

        char cmd[CMD_LEN] = "";
        sscanf(text.pointers[line], "%s", cmd);

        if (stricmp(cmd, "push") == 0) {
            int value;

            reachValue(&text.pointers[line]);

            sscanf(text.pointers[line],"%d", &value);

            code[ip] = CMD_PUSH;
            code[ip+1] = value;
            printf("value is %d\n", value);
            ip +=2;
            line++;
        }
        else if (stricmp(cmd, "add") == 0) {
            //printf("add\n");
            code[ip] = CMD_ADD;
            ip++;
            line++;
        }
        else if (stricmp(cmd, "sub") == 0) {
            code[ip] = CMD_SUB;
            ip++;
            line++;
        }
        else if (stricmp(cmd, "mul") == 0) {
            code[ip] = CMD_MUL;
            ip++;
            line++;
        }
        else if (stricmp(cmd, "div") == 0) {
            code[ip] = CMD_DIV;
            ip++;
            line++;
        }
        else if (stricmp(cmd, "out") == 0) {
            //printf("out\n");
            code[ip] = CMD_OUT;
            ip++;
            line++;
        }
        else if (stricmp(cmd, "hlt") == 0) {
           // printf("hlt\n");
            code[ip] = CMD_HLT;
            ip++;
            line++;
        }
        else if (stricmp(cmd, "in") == 0) {
            code[ip] = CMD_IN;
            ip++;
            line++;
        }
        else {
            printf("incorrect command in line %zd\n", line+1);
            return 1;
        }
    }



    FILE* a_code = NULL;
    if ((a_code = fopen(CODE, "w")) == NULL) {
        printf("can't open code file\n");
    }
    fprintf(a_code, "%s\n", ASM_SIGNATURE);
    fprintf(a_code, "%zd\n", ip);

    for (size_t i = 0; i < ip; i++) {
        fprintf(a_code,"%d ", code[i]);
    }
}

void reachValue(char** ptr){
    while (!isdigit((**ptr))) {
        (*ptr)++;
    }
}

void connectPointers(struct Text* text_ptr){
    logprint("    ##### in connectPointers() #####\n");
    ASSERT(text_ptr != NULL);

    *(text_ptr->pointers) = text_ptr->buff;
    for (size_t i = 1, j = 1; i < text_ptr->ch_count-1; i++) {
        if (text_ptr->buff[i] == '\n') {
            text_ptr->buff[i] = '\0';
            text_ptr->pointers[j++] = &(text_ptr->buff[i+1]);
        }
    }

      logprint("    ##### out connectPointers() #####\n");
}

