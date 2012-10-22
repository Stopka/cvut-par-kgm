/* 
 * File:   List.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 17:06
 */

#include "List.h"

List::List() {
}

List::List(const List& orig) {
}

List::~List() {
    this->clear();
}

void List::add(Edge* item){
    ListNode* node=new ListNode;
    node->content=item;
    if(last_node==0){
        first_node=node;
    }else{
        last_node->next=node;
    }
    last_node=node;
    size++;
}

int List::getSize(){
    return size;
}

void List::clear(){
    while(first_node!=0){
        ListNode* node=first_node;
        first_node=node->next;
        if(node==last_node){
            last_node=0;
        }
        delete node;
        size--;
    }
}

bool List::isIn(Edge* item){
    ListNode* node=first_node;
    while(node!=0){
        if(node->content==item){
            return true;
        }
        node=node->next;
    }
    return false;
}

Edge* List::getFirst(){
    return first_node->content;
}

Edge* List::getLast(){
    return last_node->content;
}

bool List::isEmpty(){
    return size==0;
}