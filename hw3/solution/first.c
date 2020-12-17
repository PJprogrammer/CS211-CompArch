#include <stdio.h>
#include <string.h>
#include "first.h"

int main(int argc, char* argv[]) {

    unsigned short x;
    char* command;
    unsigned short arg1;
    unsigned short arg2;

    char* fileName = argv[1];
    FILE* fp = fopen(fileName, "r");

    fscanf(fp, "%hu", &x);
    while(fscanf(fp,"%s\t%hu\t%hu", command, &arg1, &arg2) != EOF){
        if(strcmp(command, "set") == 0) {
            x = setNthBit(x, arg1, arg2);
            printf("%hu\n", x);
        } else if(strcmp(command, "comp") == 0) {
            x = setNthBitComplement(x, arg1);
            printf("%hu\n", x);
        }else if(strcmp(command, "get") == 0) {
            printf("%hu\n", getNthBit(x, arg1));
        }
    }

    fclose(fp);

    return 0;
}

unsigned short setNthBit(unsigned short x, unsigned short n, unsigned short v) {
    if(v == 0 && getNthBit(x, n) == 1) {
        return x & ~(1<<n);
    } else if(v == 1 && getNthBit(x, n) == 0) {
        return x | (1<<n);
    }

    return x;
}

unsigned short setNthBitComplement(unsigned short x, unsigned short n) {
    return x ^ (1<<n);
}

unsigned short getNthBit(unsigned short x, unsigned short n) {
    return (x>>n) & 1;
}