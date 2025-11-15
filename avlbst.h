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

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* x);
    void rotateRight(AVLNode<Key, Value>* x);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int diff);



};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {
  if (x == NULL) return;
  AVLNode<Key, Value>* y = x -> getRight();
  if (y == NULL) return; // no rotation necessary

  AVLNode<Key, Value>* b = y -> getLeft();
  AVLNode<Key, Value>* parent = x -> getParent();

  // y moves up
  y -> setParent(parent);
  if (parent == NULL) {
    this -> root_ = y;
  } else if (x == parent -> getLeft()) {
    parent -> setLeft(y);
  } else {
    parent -> setRight(y);
  }

  // x becomes left child of y
  y -> setLeft(x);
  x -> setParent(y);

  // b becomes right of x
  x -> setRight(b);
  if (b != NULL) {
    b -> setParent(x);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x) {
  if (x == NULL) return;
  AVLNode<Key, Value>* y = x -> getLeft();
  if (y == NULL) return; // no rotation necessary
  AVLNode<Key, Value>* b = y -> getRight();
  AVLNode<Key, Value>* parent = x -> getParent();

  // y moves up
  y -> setParent(parent);
  if (parent == NULL) {
    this -> root_ = y;
  } else if (x == parent -> getLeft()) {
    parent -> setLeft(y);
  } else {
    parent -> setRight(y);
  }

  // x is right child of y
  y -> setRight(x);
  x -> setParent(y);

  // b is left child of x
  x -> setLeft(b);
  if (b != NULL) {
    b -> setParent(x);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
  if (parent == NULL) return;
  AVLNode<Key, Value>* gp = parent -> getParent();
  if (gp == NULL) return;

  // parent is left child of gp
  if (parent == gp -> getLeft()) {
    gp -> updateBalance(1); // left subtree grew
    if (gp -> getBalance() == 0) {
      // height is unchanged
      return;
    } else if (gp -> getBalance() == 1) {
      // height incrased
      insertFix(gp, parent);
    } else if (gp -> getBalance() == 2) {
      // imbalance at gp
      if (node == parent -> getLeft()) {
        rotateRight(gp);
        parent -> setBalance(0);
        gp -> setBalance(0);
      } else {
        rotateLeft(parent);
        rotateRight(gp);
        if (node -> getBalance() == 1) {
          parent -> setBalance(0);
          gp -> setBalance(-1);
        } else if (node -> getBalance() == 0) {
          parent -> setBalance(0);
          gp -> setBalance(0);
        } else {
          parent -> setBalance(1);
          gp -> setBalance(0);
        }
        node -> setBalance(0);
      }
    }
  }
  // parent is right of gp
  else {
    gp -> updateBalance(-1); // right subtree grew
    if (gp -> getBalance() == 0) {
      return;
    } else if (gp -> getBalance() == -1) {
      insertFix(gp, parent);
    } else {
      // imbalance at gp
      if (node == parent -> getRight()) {
        rotateLeft(gp);
        parent -> setBalance(0);
        gp -> setBalance(0);
      } else {
        rotateRight(parent);
        rotateLeft(gp);
        if (node -> getBalance() == -1) {
          parent -> setBalance(0);
          gp -> setBalance(1);
        } else if (node -> getBalance() == 0) {
          parent -> setBalance(0);
          gp -> setBalance(0);
        } else {
          parent -> setBalance(-1);
          gp -> setBalance(0);
        }
        node -> setBalance(0);
      }
    }
  }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    Key key = new_item.first;
    Value val = new_item.second;

    // empty tree
    if (this -> root_ == NULL) {
      this -> root_ = new AVLNode<Key, Value>(key, val, NULL);
      return;
    }

    // find place to insert
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this -> root_);
    AVLNode<Key, Value>* parent = NULL;

    while (curr != NULL) {
      parent = curr;
      if (key < curr -> getKey()) {
        curr = curr -> getLeft();
      } else if (key > curr -> getKey()) {
        curr = curr -> getRight();
      } else {
        // exists, no rebalancing
        curr -> setValue(val);
        return;
      }
    }

    //create new node
    AVLNode<Key, Value>* node = new AVLNode<Key, Value>(key, val, parent);
    if (key < parent -> getKey()) {
      parent -> setLeft(node);
      // left subtree grew
      if (parent -> getBalance() == -1) {
        // was right, now balanced
        parent -> setBalance(0);
      } else if (parent -> getBalance() == 0) {
        // was balanced, now left-heavy, so may need to fix
        parent -> setBalance(1);
        insertFix(parent, node);
      } else if (parent -> getBalance() == 1) {
        // parent was left-heavy, now more left-heavy
        // need to fix
        parent -> setBalance(2);
        insertFix(parent, node);
      }
    } else {
      parent -> setRight(node);
      // right subtree grew
      if (parent -> getBalance() == 1) {
        // was left, now balanced
        parent -> setBalance(0);
      } else if (parent -> getBalance() == 0) {
        // was balanced, now right heavy, may need fix
        parent -> setBalance(-1);
        insertFix(parent, node);
      } else if (parent -> getBalance() == -1) {
        // was right, now more right, need fix
        parent -> setBalance(-2);
        insertFix(parent, node);
      }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff) {
  if (node == NULL) return;
  AVLNode<Key, Value>* parent = node -> getParent();
  int new_diff = 0;
  if (parent != NULL) {
    if (node == parent -> getLeft()) {
      new_diff = -1; // left subtree shrinks
    } else {
      new_diff = 1; // right subtree shrinks
    }
  }

  int bal = node -> getBalance();

  // left subtree shrinking
  if (diff == -1) {
    int new_bal = bal + diff;

    if (new_bal == -2) {
      // right-heavy, need to rotate
      AVLNode<Key, Value>* c = node -> getRight();
      int c_bal = c -> getBalance();
      if (c_bal <= 0) {
        // just left rotation
        rotateLeft(node);
        if (c_bal == 0) {
          node -> setBalance(-1);
          c -> setBalance(1);
          return; // height not changed after rotation
        } else {
          node -> setBalance(0);
          c -> setBalance(0);
          removeFix(parent, new_diff); // height decreased, go up
        }
      } else {
        // cbal == 1, need to rotate twice
        AVLNode<Key, Value>* g = c -> getLeft();
        int g_bal = g -> getBalance();
        rotateRight(c);
        rotateLeft(node);
        if (g_bal == -1) {
          node -> setBalance(1);
          c -> setBalance(0);
        } else if (g_bal == 0) {
          node -> setBalance(0);
          c -> setBalance(0);
        } else {
          node -> setBalance(0);
          c -> setBalance(-1);
        }
        g -> setBalance(0);
        removeFix(parent, new_diff);
      }
    } else {
      // no rotation, just update balance
      node -> setBalance(new_bal);
      if (new_bal == 0) {
        // height decreased, go upward
        removeFix(parent, new_diff);
      } else {
        // height not changed, stop
        return;
      }
    }
  } else if (diff == 1) { // right subtree of node decrease
    int new_bal = bal + diff;
    if (new_bal == 2) {
      AVLNode<Key, Value>* c = node -> getLeft();
      int c_bal = c -> getBalance();
      if (c_bal >= 0) {
        // just one right rotation needed
        rotateRight(node);
        if (c_bal == 0) {
          node -> setBalance(1);
          c -> setBalance(-1);
          return; // height not changed after rotation
        } else {
          node -> setBalance(0);
          c -> setBalance(0);
          removeFix(parent, new_diff);
        }
      } else {
        // double rotation needed, c_bal == -1
        AVLNode<Key, Value>* g = c -> getRight();
        int g_bal = g -> getBalance();
        rotateLeft(c);
        rotateRight(node);
        if (g_bal == 1) {
          node -> setBalance(-1);
          c -> setBalance(0);
        } else if (g_bal == 0) {
          node -> setBalance(0);
          c -> setBalance(0);
        } else {
          node -> setBalance(0);
          c -> setBalance(1);
        }
        g -> setBalance(0);
        removeFix(parent, new_diff);
      }
    } else {
      // no rotation needed, just update balance
      node -> setBalance(new_bal);
      if (new_bal == 0) {
        // height decreased, go up
        removeFix(parent, new_diff);
      } else {
        // height not changed
        return;
      }
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    // find node
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this -> internalFind(key));
    if (node == NULL) {
      // not found
      return;
    }
    // if two children, swap with predecessor
    if (node -> getLeft() != NULL && node -> getRight() != NULL) {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
      nodeSwap(node, pred);
    }
    // node now has as most 1 child
    AVLNode<Key, Value>* parent = node -> getParent();
    AVLNode<Key, Value>* child;
    if (node -> getLeft() == NULL) {
      child = node -> getRight();
    } else {
      child = node -> getLeft();
    }
    if (child != NULL) {
      child -> setParent(parent);
    }

    int diff = 0; // which side removed from

    if (parent == NULL) {
      // node was root
      this -> root_ = child;
    } else if (parent -> getLeft() == node) {
      parent -> setLeft(child);
      // left subtree shrank, diff now -1
      diff = -1;
    } else {
      parent -> setRight(child);
      // right subtree shrank, diff now +1
      diff = 1;
    }

    delete node;

    // rebalance if necessary
    if (parent != NULL) {
      removeFix(parent, diff);
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
