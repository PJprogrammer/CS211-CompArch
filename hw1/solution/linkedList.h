#pragma once

struct Node {
    struct Node* next;
    int data;
};

struct Node* insert(struct Node* head, int dataToInsert);
struct Node* delete(struct Node* head, int dataToDelete);
int size(struct Node* head);
void printLinkedList(struct Node* head);
void deallocateLinkedList(struct Node* head);
