#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "first.h"

int s;
int b;
int E;
int S;

line** cache;

bool isFifo;

unsigned long memReads = 0;
unsigned long memWrites = 0;
unsigned long cacheHits = 0;
unsigned long cacheMisses = 0;


int main(int argc, char* argv[]) {
    if(parseInput(argc, argv) == -1) {
        return -1;
    }
    initCache();

    unsigned long pc;
    char io;
    unsigned long adr;

    unsigned long tagNum;
    unsigned long setNum;

    char* fileName = argv[5];
    FILE* fp = fopen(fileName, "r");

    if(fp == NULL) {
        fprintf(stderr, "Error: Unable to open file.");
        return -1;
    }

    while(fscanf(fp, "%lx: %c %lx", &pc, &io, &adr) == 3){
        if(io == 'R' || io == 'W') {
            if(io == 'W') memWrites++;

            extractData(&tagNum, &setNum, adr);
            performCacheOperation(tagNum, setNum);
        } else {
            fprintf(stderr, "Error: Invalid IO operation.");
            continue;
        }
    }

    fclose(fp);
    freeCache();

    char* formatString = "Memory reads: %lu\nMemory writes: %lu\nCache hits: %lu\nCache misses: %lu\n";
    printf(formatString, memReads, memWrites, cacheHits, cacheMisses);

    return 0;
}

int parseInput(int argc, char* argv[]) {
    if(argc != 6) {
        fprintf(stderr, "Error: Incorrect number of arguments.");
        return -1;
    }

    int C = atoi(argv[1]);
    int B = atoi(argv[4]);

    if(C == 0 || floor(log2(C)) != log2(C)) {
        fprintf(stderr, "Error: Invalid Cache Size.");
        return -1;
    } else if(B == 0 || floor(log2(B)) != log2(B)) {
        fprintf(stderr, "Error: Invalid Block Size.");
        return -1;
    }

    if(strcmp(argv[2], "direct") == 0) {
        E = 1;
        S = C / (B * E);
    } else if(strcmp(argv[2], "assoc") == 0) {
        S = 1;
        E = C / (B * S);
    } else if(strlen(argv[2])-6 > 0) {
        char assoc[7];
        copySubstring(assoc, 6, argv[2], 0);
        char num[strlen(argv[2])-6 + 1];
        copySubstring(num, strlen(argv[2])-6, argv[2], 6);

        E = atoi(num);

        if(E == 0 || floor(log2(E)) != log2(E) || strcmp(assoc, "assoc:") != 0) {
            fprintf(stderr, "Error: Invalid Associativity.");
            return -1;
        }

        S = C / (B * E);
    } else {
        fprintf(stderr, "Error: Invalid Associativity.");
        return -1;
    }

    b = log2(B);
    s = log2(S);

    if(strcmp(argv[3], "fifo") == 0) {
        isFifo = true;
    } else if(strcmp(argv[3], "lru") == 0) {
        isFifo = false;
    } else {
        fprintf(stderr, "Error: Invalid Replacement Policy.");
        return -1;
    }

    return 0;
}

void initCache() {
    cache = (line**) calloc(S, sizeof(line*));
    for (int i = 0; i < S; ++i) {
        cache[i] = calloc(E, sizeof(line));

        for (int j = 0; j < E; ++j) {
            cache[i][j].tag = 0;
            cache[i][j].isValid = false;
        }
    }
}

void freeCache() {
    for (int i = 0; i < S; ++i) {
        free(cache[i]);
    }
    free(cache);
}

void extractData(unsigned long* tag, unsigned long* set, unsigned long adr) {
    *tag = adr >> (s+b);
    *set = (adr >> b) & ((1UL << s) - 1);
}

void performCacheOperation(unsigned long tagNum, unsigned long setNum) {
    line* set = cache[setNum];

    for (int i = 0; i < E; ++i) {
        line cline = set[i];

        if(cline.isValid == false) {
            cacheMisses++;
            memReads++;
            set[i].isValid = true;
            set[i].tag = tagNum;
            return;
        } else if(cline.tag == tagNum) {
            cacheHits++;
            if(isFifo == true) {
                return;
            } else {
                moveToEnd(i, set);
                return;
            }
        }
    }

    cacheMisses++;
    memReads++;
    set[0].tag = tagNum;
    moveToEnd(0, set);
}

void moveToEnd(int i, line set[]) {
    int end = i;
    while (end < E && set[end].isValid == true) {
        end++;
    }

    line temp = set[i];
    for (int j = i; j < end - 1; ++j) {
        set[j] = set[j+1];
    }
    set[end-1] = temp;
}

void copySubstring(char* new, int newLen, char* old, int offset) {
    memcpy(new, &old[offset], newLen);
    new[newLen] = '\0';
}