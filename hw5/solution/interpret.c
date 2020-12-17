#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "interpret.h"

int main(int argc, char* argv[]) {
    char* instructions[100];
    short reg[7];
    short rip = 0;
    
    short numInstr = loadInstructions(argv[1], instructions);

    while (rip < numInstr) {
        executeInstruction(&rip, instructions[rip], reg);
    }

    for (int i = 0; i < numInstr; ++i) {
        free(instructions[i]);
    }

    return 0;
}

void executeInstruction(short* rip, char* instruction, short reg[]) {
    struct instr myInstr = parseInstruction(instruction, reg);
    int shouldJump = 0;

    if(strcmp("noop", myInstr.command) == 0) {
        //pass
    } else if(strcmp("read", myInstr.command) == 0) {
        short temp;
        scanf("%hd", &temp);
        *myInstr.x = temp;
    } else if(strcmp("print", myInstr.command) == 0) {
        printf("%hd", *myInstr.x);
    } else if(strcmp("mov", myInstr.command) == 0) {
        *myInstr.y = *myInstr.x;
    } else if(strcmp("add", myInstr.command) == 0) {
        *myInstr.y = *myInstr.y + *myInstr.x;
    } else if(strcmp("sub", myInstr.command) == 0) {
        *myInstr.y = *myInstr.y - *myInstr.x;
    } else if(strcmp("mul", myInstr.command) == 0) {
        *myInstr.y = *myInstr.y * *myInstr.x;
    } else if(strcmp("div", myInstr.command) == 0) {
        *myInstr.y = *myInstr.x / *myInstr.y;
    } else if(strcmp("jmp", myInstr.command) == 0) {
        shouldJump = 1;
    } else if(strcmp("je", myInstr.command) == 0) {
        if(*myInstr.y == *myInstr.z) shouldJump = 1;
    } else if(strcmp("jne", myInstr.command) == 0) {
        if(*myInstr.y != *myInstr.z) shouldJump = 1;
    } else if(strcmp("jg", myInstr.command) == 0) {
        if(*myInstr.y > *myInstr.z) shouldJump = 1;
    } else if(strcmp("jge", myInstr.command) == 0) {
        if(*myInstr.y >= *myInstr.z) shouldJump = 1;
    } else if(strcmp("jl", myInstr.command) == 0) {
        if(*myInstr.y < *myInstr.z) shouldJump = 1;
    } else if(strcmp("jle", myInstr.command) == 0) {
        if(*myInstr.y <= *myInstr.z) shouldJump = 1;
    }

    if(shouldJump == 1) {
        *rip = *myInstr.x;
    } else {
        *rip = *rip+1;
    }

    free(myInstr.command);
}

struct instr parseInstruction(char* instr, short reg[]) {
    struct instr myInstr;
    char* p;
    short* creg;

    char* instruction = myStrdup(instr);

    p = strtok(instruction," ");
    int i = 0;
    while (p!= NULL && i < 4)
    {
        if (i == 0) {
            myInstr.command = myStrdup(p);
        } else if(strcmp("ax", p) == 0) {
            creg = &reg[0];
        } else if(strcmp("bx", p) == 0) {
            creg = &reg[1];
        } else if(strcmp("cx", p) == 0) {
            creg = &reg[2];
        } else if(strcmp("dx", p) == 0) {
            creg = &reg[3];
        } else {
            if(i == 1) {
                reg[4] = atoi(p);
                creg = &reg[4];
            } else if(i == 2) {
                reg[5] = atoi(p);
                creg = &reg[5];
            } else if(i == 3) {
                reg[6] = atoi(p);
                creg = &reg[6];
            }
        }

        if (i == 1) {
            myInstr.x = creg;
        } else if(i == 2) {
            myInstr.y = creg;
        } else if(i == 3) {
            myInstr.z = creg;
        }

        p = strtok (NULL, " ");
        i++;
    }

    free(instruction);

    return myInstr;
}

short loadInstructions(char* fname, char* instructions[]) {
    char* line = NULL;
    size_t len = 0;

    FILE* fp = fopen(fname, "r");

    short i = 0;
    while (getline(&line, &len, fp) != -1) {
        if (line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }

        if(strlen(line) == 0) {
            instructions[i] = myStrdup("noop");
        } else {
            instructions[i] = myStrdup(line);
        }

        i++;
    }

    free(line);
    fclose(fp);

    return i;
}

char* myStrdup(const char *src) {
    char *dst = malloc(strlen (src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}