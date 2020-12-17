#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hashTable.h"
#define BUCKETS 10000


int main(int argc, char* argv[]) {
    char* fname = argv[1];
    if(access( fname, F_OK ) == -1) {
        printf("error");
        return -1;
    }
    FILE* fp = fopen(fname, "r");

    struct Node* hashTable[BUCKETS];
    for (int i = 0; i < BUCKETS; i++) {
        hashTable[i] = NULL;
    }
    int numCollision = 0;
    int numHits = 0;

    char op;
    int num;
    while (fscanf(fp, "%c %d\n", &op, &num) == 2) {
        if(op == 'i') {
            numCollision += insertValue(hashTable, num);
        } else if(op == 's') {
            numHits += search(hashTable, num);
        }
    }
    fclose(fp);

    printf("%d\n%d", numCollision, numHits);

    deallocateHashTable(hashTable);

    return 0;
}

int findHash(int value) {
    int hash = value % BUCKETS;
    if(value < 0) {
        hash = hash + BUCKETS;
    }

    return hash;
}

int insertValue(struct Node* buckets[], int value) {
    int hash = findHash(value);
    struct Node* bucket = buckets[hash];

    if(bucket == NULL) {
        struct Node* newValue = NULL;
        newValue = (struct Node*) malloc(sizeof(struct Node));
        newValue->data = value;
        newValue->next = NULL;
        buckets[hash] = newValue;
        return 0;
    } else {
        buckets[hash] = insertNode(bucket, value);
        return 1;
    }

}

int search(struct Node* buckets[], int value) {
    int hash = findHash(value);

    return findNode(buckets[hash], value);
}

struct Node* insertNode(struct Node* head, int dataToInsert) {
    struct Node* temp = head;

    struct Node* newNode = NULL;
    newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = dataToInsert;

    if(temp == NULL || dataToInsert <= temp->data) {
        newNode->next = temp;
        return newNode;
    } else if(temp != NULL) {
        while (temp->next != NULL) {
            if(dataToInsert <= temp->next->data) {
                newNode->next = temp->next;
                temp->next = newNode;
                return head;
            }
            temp = temp->next;
        }

        newNode->next = NULL;
        temp->next = newNode;
        return head;
    }

    return head; // Will not reach here (line needed to fix warnings)
}

int findNode(struct Node* head, int target) {
    while (head != NULL) {
        if(target == head->data) {
            return 1;
        }
        head = head->next;
    }

    return 0;
}


void deallocateHashTable(struct Node* buckets[]) {
    for (int i = 0; i < BUCKETS; i++) {
        deallocateLinkedList(buckets[i]);
    }
}

void deallocateLinkedList(struct Node* head) {
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}
