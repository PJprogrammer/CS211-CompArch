#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bst.h"

int main(int argc, char* argv[]) {
    char* fname = argv[1];
    if(access( fname, F_OK ) == -1) {
        printf("error");
        return -1;
    }
    FILE* fp = fopen(fname, "r");

    struct BSTNode *head = NULL;

    char op;
    int num;
    while (fscanf(fp, "%c %d\n", &op, &num) == 2) {
        head = insert(head, num);
    }

    inOrderTraversal(head);
    freeBST(head);

    fclose(fp);
    return 0;
}

struct BSTNode* insert (struct BSTNode* head, int dataToInsert) {
    struct BSTNode* prev = NULL;
    struct BSTNode* temp = head;

    while (temp != NULL) {
        prev = temp;
        if(dataToInsert < temp->data) {
            temp = temp->left;
        } else if (dataToInsert > temp->data) {
            temp = temp->right;
        } else {
            return head;
        }
    }

    struct BSTNode* newNode = NULL;
    newNode = (struct BSTNode*) malloc(sizeof(struct BSTNode));
    newNode->data = dataToInsert;
    newNode->right = NULL;
    newNode->left = NULL;

    if(prev == NULL) {
        return newNode;
    }

    if(dataToInsert < prev->data) {
        prev->left = newNode;
    } else if(dataToInsert > prev->data) {
        prev->right = newNode;
    }

    return head;
}

void inOrderTraversal(struct BSTNode* head) {
    if(head == NULL) {
        return;
    }

    inOrderTraversal(head->left);
    printf("%d\t", head->data);
    inOrderTraversal(head->right);
}

void freeBST(struct BSTNode* head) {
    if(head == NULL) {
        return;
    }

    freeBST(head->left);
    freeBST(head->right);
    free(head);
}
