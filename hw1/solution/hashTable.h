#pragma once

struct Node {
    struct Node* next;
    int data;
};

int findHash(int value);

int insertValue(struct Node* buckets[], int value);
int search(struct Node* buckets[], int value);

struct Node* insertNode(struct Node* head, int dataToInsert);
int findNode(struct Node* head, int target);

void deallocateHashTable(struct Node* buckets[]);
void deallocateLinkedList(struct Node* head);
