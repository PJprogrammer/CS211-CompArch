#pragma once

struct instr {
    char* command;
    short* x;
    short* y;
    short* z;
};
short loadInstructions(char* fname, char* instructions[]);
void executeInstruction(short* rip, char* instruction, short reg[]);
struct instr parseInstruction(char* instr, short reg[]);
char* myStrdup(const char *src);