/* 
 * File:   StackItem.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 16:37
 */

#ifndef STACKITEM_H
#define	STACKITEM_H

#include "List.h"
#include <iostream>
#include <string>
using namespace std;

class StackItem {
    friend ostream& operator<<(ostream& os, const StackItem& s);
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
    string StackItem::toString();
    void removeEdge(Edge* edge);
    void removeLastEdge();
    Edge* getEdge(int index);
    //void setVertexDegree(int vertex, int degree)
    void countDegree();
    int getMaxDegree();
    List* getPath();
    int* getVertexArray();
private:
    List* path;
    int pathDeegre;
    int vertex[];
};
#endif	/* STACKITEM_H */

