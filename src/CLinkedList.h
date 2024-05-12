/***************************************************************************************
** File Name  :         CLinkedList.h
** Author     :         WJTE
** Date       :         5/9/2024
** Description:         Item, node, and list classes for creating linked lists of items
**                      in the players inventory. These lists could be highly modular,
**                      so we cannot use an array. The implementations for Node & Item
**                      live here.
***************************************************************************************/

#ifndef CLinkedList_h
#define CLinkedList_h
#include <Arduino.h>
#include "CPipBoyPageNames.h"

// Item class, basic item data structure. add more fields as needed
class Item {
  public:
  char* name;
  uint8_t quantity;
  Item() {
    name = "unnamed item";
    quantity = 1;
  }
  Item(char* n, uint8_t q) {
    this->name = n;
    this->quantity = q;
  }

  bool operator==(Item* other) {
    // add more fields to compare as needed
    return
      this->name == other->name &&
      this->quantity == other->quantity;
  }

  // if we add more fields that are pointers, we must free that memory in here
  ~Item() {}
};

class Node {
  public:
  Item* data;
  Node* next;
  Node() {
    data = nullptr;
    next = nullptr;
  }
  Node(Item* data) {
    this->data = data;
    this->next = nullptr;
  }

  ~Node() {
    if (this->data != nullptr) {
      // we must delete the data ptr to avoid leaking memory
      delete this->data;
    }
  }
};

class LinkedList {
  Node* head;
  Node* loopPtr;

  public:
  LinkedList() { head = nullptr; loopPtr = nullptr; }

  // methods
  void push_back(Item*);
  void print();
  void deleteAt(int);
  void deleteByValue(Item*);
  void reduceQuantityAt(int, uint8_t);
  void increaseQuantityAt(int, uint8_t);

  // getters
  int length();
  Item* at(int);
  Item* loop();
  Item* nextItem();
  Node* getTail();
};

#endif