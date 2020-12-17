#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
    unsigned short x = atoi(argv[1]);
    int pairs = 0;
    int isPair = -1;
    int num1s = 0;

    do{
        if(x & 1 == 1) {
            isPair++;
            num1s++;
        } else {
            isPair = -1;
        }

        if(isPair == 1) {
            pairs++;
            isPair = -1;
        }
        x = x >> 1;
    } while (x != 0);

    if(num1s % 2 == 0) {
        printf("Even-Parity\t%d", pairs);
    } else {
        printf("Odd-Parity\t%d", pairs);
    }

    return 0;
}
