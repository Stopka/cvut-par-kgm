/* 
 * File:   Stack.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 16:25
 */

#ifndef STACK_H
#define	STACK_H

#include "StackItem.h"


struct StackNode {
    StackItem* content;
    StackNode* next;
};

class Stack {
    friend ostream& operator<<(ostream& os, const Stack& e);
private:
    StackNode* first_node;
    int size;

public:
    Stack();
    ~Stack();
    void push(StackItem* item);
    StackItem* pop();
    StackItem* popLast();
    bool is_empty() const;
    void clear();
    int count() const;
    int getSize();
};

#endif	/* STACK_H */

