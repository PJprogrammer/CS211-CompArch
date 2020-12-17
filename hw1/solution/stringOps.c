#include <stdio.h>
#include "stringOps.h"

int main(int argc, char* argv[]) {

    for (int i = 1; i < argc; i++) {
        for(char* s = argv[i]; *s != '\0'; s++) {
            if(isVowel(*s)) {
                printf("%c", *s);
            }
        }
    }

    return 0;
}

int isVowel(char letter) {
    int numVowels = 10;
    char vowels[] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
    for (int i = 0; i < numVowels; i++) {
        if(letter == vowels[i])
            return 1;
    }

    return 0;
}
