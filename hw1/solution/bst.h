#pragma once

struct BSTNode {
    struct BSTNode* left;
    struct BSTNode* right;
    int data;
};

struct BSTNode* insert(struct BSTNode* head, int dataToInsert);
void inOrderTraversal(struct BSTNode* head);
void freeBST(struct BSTNode* head);
