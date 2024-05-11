/***************************************************************************************
** File Name  :         CLinkedList.cpp
** Author     :         WJTE
** Date       :         5/9/2024
** Description:         Implementation of our linked list class
***************************************************************************************/

#include "CLinkedList.h"
#include <Arduino.h>

void LinkedList::deleteAt(int index) {
  if (head == nullptr || index > length()) return;
  loopPtr = head;

  Node *temp1 = head, *temp2 = nullptr;
  if (index == 0) {
    head = head->next;
    delete temp1;
    return;
  }

  while (index-- > 0) {
    temp2 = temp1;
    temp1 = temp1->next;
  }

  temp2->next = temp1->next;
  delete temp1;
}

Node* LinkedList::getTail() {
  if (head == nullptr) return head;
  Node* curr = head;
  while (curr->next != nullptr) curr = curr->next;
  return curr;
}

void LinkedList::push_back(Item* data) {
  Node *newNode = new Node(data);
  if (head == nullptr) {
    head = newNode;
    return;
  }

  getTail()->next = newNode;
}

void LinkedList::deleteByValue(Item *item) {
  if (head == nullptr) return;

  int currentIndex = 0;
  Node *curr = head;
  while (curr != nullptr) {
    if (
      item->name == curr->data->name &&
      item->quantity == curr->data->quantity
      // todo: add more here
    ) {
      deleteAt(currentIndex);
      return;
    }
    curr = curr->next;
    currentIndex++;
  }
}

void LinkedList::reduceQuantityAt(int index, uint8_t amount) {
  if (head == nullptr || index > length()) return;

  Item *item = at(index);
  if (item->quantity <= amount) {
    deleteAt(index);
  } else {
    item->quantity -= amount;
  }
}

void LinkedList::increaseQuantityAt(int index, uint8_t amount) {
  if (head == nullptr || index > length()) return;

  Item *item = at(index);
  uint8_t MAX_SHORT = 0 - 1;
  if (item->quantity + amount > MAX_SHORT) {
    item->quantity = MAX_SHORT;
  } else {
    item->quantity += amount;
  }
}

int LinkedList::length() {
  if (head == nullptr) return 0;
  int len = 0;
  Node *temp1 = head;
  while (temp1 != nullptr) {
    temp1 = temp1->next;
    len++;
  }
  return len;
}

Item* LinkedList::at(int index) {
  if (head == nullptr || index > length() || index < 0) return nullptr;
  Node *curr = head;
  int count = 0;
  while (count < index) {
    count++;
    curr = curr->next;
  }
  return curr->data;
}

Item* LinkedList::loop() {
  loopPtr = head;
  if (loopPtr == nullptr) return nullptr;
  return loopPtr->data;
}

Item* LinkedList::nextItem() {
  if (head == nullptr || loopPtr == nullptr) return nullptr;
  if (loopPtr->next == nullptr) return nullptr;
  loopPtr = loopPtr->next;
  return loopPtr->data;
}

void LinkedList::print() {
  Node *temp = head;
  while (temp != nullptr) {
    Serial.print("List val: "); Serial.println(temp->data->name);
    temp = temp->next;
  }
}
