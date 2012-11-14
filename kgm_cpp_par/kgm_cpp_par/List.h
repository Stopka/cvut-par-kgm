/* 
 * File:   List.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 17:06
 */

#ifndef LIST_H
#define	LIST_H

#include "Edge.h"
#include <iostream>
using namespace std;
class ListNode{
    public:
        ListNode(){
            next=NULL;
            content=NULL;
        }
    ListNode* next;
    Edge* content;
};

class List {
    friend ostream& operator<<(ostream& os, const List& s);
public:
    List();
    List(const List& orig);
    virtual ~List();
    void add(Edge* item);
    bool isIn(Edge* item);
    int getSize();
    void clear();
    Edge* findById(int id);
    Edge* getFirst();
    Edge* getLast();
    bool isEmpty();
    void remove(Edge* edge);
    void removeLast();
    Edge* getItem(int index);
private:
    int size;
    ListNode* first_node;
    ListNode* last_node;
};

#endif	/* LIST_H */

