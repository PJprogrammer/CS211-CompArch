#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "linkedList.h"

int main(int argc, char* argv[]) {
    char* fname = argv[1];
    if(access( fname, F_OK ) == -1) {
        printf("error");
        return -1;
    }
    FILE* fp = fopen(fname, "r");

    struct Node *head = NULL;

    char op;
    int num;
    while (fscanf(fp, "%c %d\n", &op, &num) == 2) {
        if(op == 'i') {
            head = insert(head, num);
        } else if(op == 'd') {
            head = delete(head, num);
        }
    }

    printf("%d\n", size(head));
    printLinkedList(head);

    deallocateLinkedList(head);
    fclose(fp);

    return 0;
}

struct Node* insert(struct Node* head, int dataToInsert) {
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

struct Node* delete(struct Node* head, int dataToDelete) {
    struct Node* temp = head;
    if(temp != NULL && temp->data == dataToDelete) {
        struct Node* nextNode = temp->next;
        free(temp);
        return nextNode;
    } else if(temp != NULL) {
        while (temp->next != NULL) {
            if(temp->next->data == dataToDelete) {
                struct Node* nextNode = temp->next->next;
                free(temp->next);
                temp->next = nextNode;
                return head;
            }
            temp = temp->next;
        }
    }

    return head;
}

int size(struct Node* head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

void printLinkedList(struct Node* head) {
    int prevNum;
    if(head != NULL) {
        prevNum = head->data;
        printf("%d\t", head->data);
        head = head->next;
    }

    while (head != NULL) {
        if(prevNum != head->data) {
            printf("%d\t", head->data);
        }

        prevNum = head->data;
        head = head->next;
    }
}

void deallocateLinkedList(struct Node* head) {
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}
