#include <stdio.h>
#include <stdlib.h>
#include "third.h"

int main(int argc, char* argv[]) {
    unsigned short x = atoi(argv[1]);
    unsigned short i = 0;
    unsigned short j = (sizeof(unsigned short)*8)-1;

    while (i < j) {
        if(getNthBit(x, i) != getNthBit(x, j)) {
            printf("Not-Palindrome");
            return 0;
        }
        i++;
        j--;
    }

    printf("Is-Palindrome");

    return 0;
}

unsigned short getNthBit(unsigned short x, unsigned short n) {
    return (x>>n) & 1;
}