/* 
 * File:   List.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 17:06
 */

#ifndef LIST_H
#define	LIST_H

#include "Edge.h"

struct ListNode{
    ListNode* next=0;
    Edge content=0;
};

class List {
public:
    List();
    List(const List& orig);
    virtual ~List();
    void add(Edge* item);
    bool isIn(Edge* item);
    int getSize();
    void clear();
    Edge* getFirst();
    Edge* getLast();
    bool isEmpty();
private:
    int size=0;
    ListNode* first_node=0;
    ListNode* last_node=0;
};

#endif	/* LIST_H */

