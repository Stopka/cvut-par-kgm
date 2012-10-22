/* 
 * File:   StackItem.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 16:37
 */

#ifndef STACKITEM_H
#define	STACKITEM_H

#include "List.h"
using namespace std;

class StackItem {
public:
    StackItem(List* list);
    StackItem(const StackItem& orig);
    virtual ~StackItem();
    void addEdge(Edge* e);
    bool isContainsEdge(Edge* e);
    int pathSize();
    Edge* getLastEdge();
    Edge* getFristEdge();
    bool isPathEmpty();
    
private:
    List* path;
    int pathDeegre;
    int vertex[];
};

#endif	/* STACKITEM_H */

