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
private:
    StackNode* first_node;
    int size;

public:
    Stack();
    ~Stack();
    void push(StackItem* item);
    StackItem* pop();
    bool is_empty() const;
    void clear();
    int count() const;
};

#endif	/* STACK_H */

