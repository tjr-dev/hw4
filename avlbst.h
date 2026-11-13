#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;
    

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    bool rebalanceAt(AVLNode<Key, Value>* node);
    using BinarySearchTree<Key, Value>::nodeSwap;

    // Add helper functions here


};


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* ptr = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* middle = ptr->getLeft();

    // 
    ptr->setParent(parent);

    if (parent == nullptr) {
        this->root_ = ptr;
    } else if (parent->getLeft() == node) {
        parent->setLeft(ptr);
    } else {
        parent->setRight(ptr);
    }

    // rotate
    ptr->setLeft(node);
    node->setParent(ptr);

    // middle subtere kept
    node->setRight(middle);
    if (middle != nullptr) {
        middle->setParent(node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* node)
{
    // TODO
    AVLNode<Key, Value>* ptr = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* middle = ptr->getRight();

    // connect promoted node to original parent
    ptr->setParent(parent);

    if (parent == nullptr) {
        this->root_ = ptr;
    } else if (parent->getLeft() == node) {
        parent->setLeft(ptr);
    } else {
        parent->setRight(ptr);
    }

    // erotate
    ptr->setRight(node);
    node->setParent(ptr);

    // preserve middle subtree
    node->setLeft(middle);
    if (middle != nullptr) {
        middle->setParent(node);
    }
}

template<class Key, class Value>
bool AVLTree<Key, Value>::rebalanceAt(AVLNode<Key, Value>* node)
{
    if (node->getBalance() == -2) {
        AVLNode<Key, Value>* left = node->getLeft();
        int b = left->getBalance();

        if (b <= 0) {
            rotateRight(node);

            if (b == 0) {
                node->setBalance(-1);
                left->setBalance(1);
                return false;
            }

            node->setBalance(0);
            left->setBalance(0);
        } else {
            AVLNode<Key, Value>* middle = left->getRight();
            int b = middle->getBalance();

            rotateLeft(left);
            rotateRight(node);

            if (b == -1) {
                left->setBalance(0);
                node->setBalance(1);
            } else if (b == 1) {
                left->setBalance(-1);
                node->setBalance(0);
            } else {
                left->setBalance(0);
                node->setBalance(0);
            }

            middle->setBalance(0);
        }

        return true;
    }

    if (node->getBalance() == 2) {
        AVLNode<Key, Value>* right = node->getRight();
        int b = right->getBalance();

        if (b >= 0) {
            rotateLeft(node);

            if (b == 0) {
                node->setBalance(1);
                right->setBalance(-1);
                return false;
            }

            node->setBalance(0);
            right->setBalance(0);
        } else {
            AVLNode<Key, Value>* middle = right->getLeft();
            int b = middle->getBalance();

            rotateRight(right);
            rotateLeft(node);

            if (b == -1) {
                node->setBalance(0);
                right->setBalance(1);
            } else if (b == 1) {
                node->setBalance(-1);
                right->setBalance(0);
            } else {
                node->setBalance(0);
                right->setBalance(0);
            }

            middle->setBalance(0);
        }

        return true;
    }

    return false;
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(
    const std::pair<const Key, Value>& new_item)
{
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(
            new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* curr =
        static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key, Value>* parent = nullptr;

    // Ffind insertion position
    while (curr != nullptr) {
        parent = curr;

        if (new_item.first < curr->getKey()) {
            curr = curr->getLeft();
        } else if (new_item.first > curr->getKey()) {
            curr = curr->getRight();
        } else {
            curr->setValue(new_item.second);
            return;
        }
    }

    // create new node
    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(
        new_item.first, new_item.second, parent);

    if (new_item.first < parent->getKey()) {
        parent->setLeft(n);
    } else {
        parent->setRight(n);
    }

    // upddate balance factors upward
    AVLNode<Key, Value>* child = n;

    while (parent != nullptr) {
        if (parent->getLeft() == child) {
            parent->updateBalance(-1);
        } else {
            parent->updateBalance(1);
        }

        int balance = parent->getBalance();

        if (balance == 0) {
            break;
        }

        if (balance == -2 || balance == 2) {
            rebalanceAt(parent);
            break;
        }

        child = parent;
        parent = parent->getParent();
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* curr =
        static_cast<AVLNode<Key, Value>*>(
            this->internalFind(key));

    if (curr == nullptr) {
        return;
    }

    // if 2 children swap with pred
    if (curr->getLeft() != nullptr &&
        curr->getRight() != nullptr) {

        AVLNode<Key, Value>* pred = curr->getLeft();

        while (pred->getRight() != nullptr) {
            pred = pred->getRight();
        }

        this->nodeSwap(curr, pred);
    }

    // nmow curr has at most one child
    AVLNode<Key, Value>* parent = curr->getParent();

    bool removedLeft =
        parent != nullptr && parent->getLeft() == curr;

    AVLNode<Key, Value>* child;

    if (curr->getLeft() != nullptr) {
        child = curr->getLeft();
    } else {
        child = curr->getRight();
    }

    // reconnect child and parent
    if (child != nullptr) {
        child->setParent(parent);
    }

    if (parent == nullptr) {
        this->root_ = child;
    } else if (removedLeft) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete curr;

    // update the balance factors upward
    while (parent != nullptr) {
        AVLNode<Key, Value>* grandparent =
            parent->getParent();

        bool parentWasLeft =
            grandparent != nullptr &&
            grandparent->getLeft() == parent;

        if (removedLeft) {
            parent->updateBalance(1);
        } else {
            parent->updateBalance(-1);
        }

        int balance = parent->getBalance();

        if (balance == 1 || balance == -1) {
            break;
        }

        if (balance == 2 || balance == -2) {
            bool heightDecreased = rebalanceAt(parent);

            if (!heightDecreased) {
                break;
            }
        }

        removedLeft = parentWasLeft;
        parent = grandparent;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
