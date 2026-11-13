#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool isLeaf(Node * root) {
    if (root == nullptr) {
        return false;
    }
    if (root->left == nullptr && root->right == nullptr) { // if no children
        return true;
    }

    return false;
}

bool isFull(Node * root) {
    if (root == nullptr) {
        return false;
    }
    if (root->left != nullptr && root->right != nullptr) { // if no children
        return true;
    }

    return false;
}

int commonLeafDistance(Node * root) {
    if (root == nullptr) { // subtree empty
        return -1;
    }
    if (isLeaf(root)) {
        return 0;
    }

    
    int leftDistance = commonLeafDistance(root->left);
    int rightDistance = commonLeafDistance(root->right);

    if (leftDistance == -2 || rightDistance == -2) {
        return -2;
    }

    if (leftDistance == -1) {
        return rightDistance + 1;
    } 

    if (rightDistance == -1) {
        return leftDistance + 1;
    }

    if (leftDistance != rightDistance) {
        return -2;
    }

    return leftDistance + 1;

}


bool equalPaths(Node * root)
{   
    return commonLeafDistance(root) != -2;
}

