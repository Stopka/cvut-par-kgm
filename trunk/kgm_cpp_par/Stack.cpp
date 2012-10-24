/* 
 * File:   Stack.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 16:25
 */

#include "Stack.h"

Stack::Stack(){
    first_node=NULL;
    size=0;
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
    //Vypis
    //cout<<"PUSH: "<<(*item)<<" |"<<item->getMaxDegree()<<endl<<(*this);
}

StackItem* Stack::pop() {
    StackNode* node=first_node;
    first_node=node->next;
    StackItem* item = node->content;

    size--;
    delete node;
    //Vypis
    //cout<<"POP: "<<(*item)<<" |"<<item->getMaxDegree()<<endl<<(*this);
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

ostream& operator<<(ostream& os,const Stack& e){
    os<<"|||||STACK||||||||"<<endl;
    StackNode* node=e.first_node;
    while(node!=NULL){
        os<<*(node->content)<<endl;
        node=node->next;
    }
    os<<endl;
    return os;
}