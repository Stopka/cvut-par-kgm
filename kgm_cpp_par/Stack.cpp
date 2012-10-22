/* 
 * File:   Stack.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 16:25
 */

#include "Stack.h"

Stack::Stack(){
}

//destruktor

Stack::~Stack() {
    clear();
}

void Stack::push(StackItem* item) {
    StackNode* node = new StackNode();
    node->content = item;
    node->next = first_node;
    first_node = node;

    size++;
}

StackItem* Stack::pop() {
    StackNode* node=first_node;
    first_node=node->next;
    StackItem* item = node->content;

    size--;
    delete node;
    return item;
}

void Stack::clear() {
    while (!is_empty())
        pop();
}

bool Stack::is_empty() const {
    return size == 0;
}

int Stack::count() const {
    return size;
}