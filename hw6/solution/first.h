#pragma once

typedef char bool;
enum boolean { false, true };

typedef struct line {
    unsigned long tag;
    bool isValid;
} line;

int parseInput(int argc, char* argv[]);

void initCache();
void freeCache();

void extractData(unsigned long* tag, unsigned long* set, unsigned long adr);
void performCacheOperation(unsigned long tagNum, unsigned long setNum);

void moveToEnd(int i, line set[]);
void copySubstring(char* new, int newLen, char* old, int offset);