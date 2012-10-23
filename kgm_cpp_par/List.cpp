/* 
 * File:   List.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 17:06
 */

#include "List.h"
#include "StackItem.h"

List::List() {
    first_node=NULL;
    last_node=NULL;
    size=0;
}

List::List(const List& orig) {
}

List::~List() {
    this->clear();
}

void List::add(Edge* item) {
    ListNode* node = new ListNode;
    node->content = item;
    if (last_node == 0) {
        first_node = node;
    } else {
        last_node->next = node;
    }
    last_node = node;
    size++;
}

int List::getSize() {
    return size;
}

void List::clear() {
    while (first_node != NULL) {
        remove(first_node->content);
    }
}

bool List::isIn(Edge* item) {
    ListNode* node = first_node;
    while (node != 0) {
        if (node->content == item) {
            return true;
        }
        node = node->next;
    }
    return false;
}

Edge* List::getFirst() {
    return first_node->content;
}

Edge* List::getLast() {
    return last_node->content;
}

bool List::isEmpty() {
    return size == 0;
}

ostream& operator<<(ostream& os, const List& s) {
    ListNode* node = s.first_node;
    bool inter = false;
    os << "[";
    while (node != 0) {
        if (inter) {
            os << "; ";
        }
        os << node->content;
        node = node->next;
        inter = true;
    }
    os << "]";
    return os;
}

void List::remove(Edge* edge) {
    ListNode* prev = 0;
    ListNode* node = first_node;
    while (node != 0) {
        if (node->content == edge) {
            if (prev != 0) {
                prev->next = node->next;
            } else {
                first_node = node->next;
            }
            if (last_node == node) {
                last_node = prev;
            }
            delete(node);
            size--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

void List::removeLast() {
    remove(last_node->content);
}

Edge* List::getItem(int index) {
    ListNode* node = first_node;
    for (int i = index; i > 0; i--) {
        if (node == 0) {
            return 0;
        }
        node = node->next;
    }
    if (node == 0) {
        return 0;
    }
    return node->content;
}