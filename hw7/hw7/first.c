#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "first.h"

int commandsMax = INIT_INSTR_LEN;
int commandsLen = 0;
instr* commands;

int numInputs;
var* inputs;
int numOutputs;
var* outputs;
varNode* head;

int** truthTable;
int numRows;

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "No file provided");
        exit(1);
    }

    loadInstructions(argv[1]);

    fillTruthTable();

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numInputs; ++j) {
            inputs[j].val = truthTable[i][j];
        }
        for (int j = 0; j < commandsLen; ++j) {
            executeInstruction(commands[j]);
        }
        for (int j = 0; j < numOutputs; ++j) {
            truthTable[i][numInputs+j] = outputs[j].val;
        }
    }

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numInputs + numOutputs; ++j) {
            printf("%d ", truthTable[i][j]);
        }
        printf("\n");
    }

    freeMem();
    return 0;
}

void loadInstructions(char* fname) {
    char* line = NULL;
    size_t len = 0;

    FILE* fp = fopen(fname, "r");
    if(fp == NULL) fatalError("Error: Can not open file");

    int i = 0;
    while (getline(&line, &len, fp) != -1) {
        removeNewline(line);

        if(i == 0) {
            char* p = strtok(line," ");
            if(strcmp(p, "INPUTVAR") != 0) fatalError(NULL);

            numInputs = atoi(strtok(NULL," "));
            if(numInputs == 0) fatalError(NULL);

            inputs = calloc(numInputs, sizeof(var));

            for (int j = 0; j < numInputs; ++j) {
                p = strtok(NULL, " ");
                inputs[j].name = myStrdup(p);
            }
        } else if(i == 1) {
            char* p = strtok(line," ");
            if(strcmp(p, "OUTPUTVAR") != 0) fatalError(NULL);

            numOutputs = atoi(strtok(NULL," "));
            if(numOutputs == 0) fatalError(NULL);

            outputs = calloc(numOutputs, sizeof(var));

            for (int j = 0; j < numOutputs; ++j) {
                p = strtok(NULL, " ");
                outputs[j].name = myStrdup(p);
            }

            commands = calloc(commandsMax, sizeof(instr));
        } else {
            parseInstruction(line);
        }
        i++;
    }

    free(line);
    fclose(fp);
}

void parseInstruction(char* line) {
    instr cInstr;

    char* p = strtok(line," ");
    if(strlen(p) >= 5) fatalError("Error: Invalid operation");
    strcpy(cInstr.op, p);

    p = strtok(NULL, " ");
    cInstr.in1 = getAdr(p, 1);

    if(strcmp(cInstr.op, "NOT") != 0) {
        p = strtok(NULL, " ");
        cInstr.in2 = getAdr(p, 1);
    }

    p = strtok(NULL, " ");
    cInstr.out = getAdr(p, 0);

    appendInstr(cInstr);
}

void executeInstruction(instr command) {
    if(strcmp(command.op, "NOT") == 0) {
        if(*command.in1 == 0) {
            *command.out = 1;
        } else {
            *command.out = 0;
        }
    } else if(strcmp(command.op, "AND") == 0) {
        if(*command.in1 == 1 && *command.in2 == 1) {
            *command.out = 1;
        } else {
            *command.out = 0;
        }
    } else if(strcmp(command.op, "OR") == 0) {
        if(*command.in1 == 1 || *command.in2 == 1) {
            *command.out = 1;
        } else {
            *command.out = 0;
        }
    } else if(strcmp(command.op, "NAND") == 0) {
        if(*command.in1 == 1 && *command.in2 == 1) {
            *command.out = 0;
        } else {
            *command.out = 1;
        }
    } else if(strcmp(command.op, "NOR") == 0) {
        if(*command.in1 == 1 || *command.in2 == 1) {
            *command.out = 0;
        } else {
            *command.out = 1;
        }
    } else if(strcmp(command.op, "XOR") == 0) {
        if((*command.in1 == 1 && *command.in2 == 0) || (*command.in1 == 0 && *command.in2 == 1)) {
            *command.out = 1;
        } else {
            *command.out = 0;
        }
    } else {
        fatalError("Error: Invalid operation");
    }
}

int* getAdr(char* p, int isInputVar) {
    for (int i = 0; i < numInputs; ++i) {
        if(strcmp(inputs[i].name, p) == 0) {
            return &inputs[i].val;
        }
    }

    if(isInputVar == 0) {
        for (int i = 0; i < numOutputs; ++i) {
            if(strcmp(outputs[i].name, p) == 0) {
                return &outputs[i].val;
            }
        }
    }

    varNode* temp = head;
    varNode* prev = head;

    while (temp != NULL) {
        if(strcmp(temp->name, p) == 0) {
            return &temp->val;
        }
        prev = temp;
        temp = temp->next;
    }

    if(isInputVar == 1) fatalError("Error: Trying to use undeclared variable");

    varNode *newNode = (varNode*) calloc(1, sizeof(varNode));
    newNode->name = myStrdup(p);

    if(prev == NULL) {
        head = newNode;
    } else {
        prev->next = newNode;
    }

    return &newNode->val;
}

void fillTruthTable() {
    numRows = pow(2, numInputs);
    truthTable = (int**) calloc(numRows, sizeof(int*));
    for (int i = 0; i < numRows; ++i) {
        truthTable[i] = (int*) calloc(numInputs + numOutputs, sizeof(int));
    }

    for (int i = 0; i < numRows; ++i) {
        int num = i;
        for (int j = numInputs-1; j >= 0; --j) {
            truthTable[i][j] = num & 1;
            num = num >> 1;
        }
    }
}

void appendInstr(instr cInstr) {
    if(commandsLen+1 < commandsMax) {
        commands[commandsLen] = cInstr;
    } else {
        commands = realloc(commands, commandsMax*2*sizeof(instr));
        commandsMax *= 2;
        commands[commandsLen] = cInstr;
    }

    commandsLen++;
}

void removeNewline(char* line) {
    int len = strlen(line);
    if(len > 1 && line[len-1] == '\n' && line[len-2] == '\r') {
        line[len-1] = '\0';
        line[len-2] = '\0';
    } else if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }
}

char* myStrdup(const char *str) {
    char *newStr = malloc(strlen (str) + 1);
    if (newStr == NULL) return NULL;
    strcpy(newStr, str);
    return newStr;
}

void fatalError(char* msg) {
    if(msg != NULL) {
        fprintf(stderr, "%s", msg);
    } else {
        fprintf(stderr, "Error: File not formatted properly");
    }

    exit(1);
}

void freeMem() {
    free(commands);

    for (int i = 0; i < numInputs; ++i) {
        free(inputs[i].name);
    }
    free(inputs);

    for (int i = 0; i < numOutputs; ++i) {
        free(outputs[i].name);
    }
    free(outputs);

    varNode* temp = head;
    varNode* prev;
    while (temp != NULL) {
        prev = temp;
        temp = temp->next;
        free(prev->name);
        free(prev);
    }

    for (int i = 0; i < numRows; ++i) {
        free(truthTable[i]);
    }
    free(truthTable);
}
