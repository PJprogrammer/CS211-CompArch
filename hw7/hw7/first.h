#pragma once

#define INIT_INSTR_LEN 5

typedef struct var {
    char* name;
    int val;
} var;

typedef struct varNode {
    char* name;
    int val;
    struct varNode* next;
} varNode;

typedef struct instr {
    int* in1;
    int* in2;
    char op[5];
    int* out;
} instr;

void loadInstructions(char* fname);
void parseInstruction(char* line);
void executeInstruction(instr command);
int* getAdr(char* p, int isInputVar);

void fillTruthTable();

void appendInstr(instr cInstr);
void removeNewline(char* line);
char* myStrdup(const char *str);
void fatalError(char* msg);
void freeMem();
