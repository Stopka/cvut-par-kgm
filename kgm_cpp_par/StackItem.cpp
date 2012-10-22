/* 
 * File:   StackItem.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 15:04
 */

#include "StackItem.h"
#include "Stack.h"

StackItem::StackItem(List* list) {
    path = list; //(list<Edge>) list.clone();
    vertex = new int[Main.NUMBER_OF_VERTEX];
    //jelikoz z kazdeho vrcholu vychazi hrana, tak ma minimalne stupen 1
    for (int i = 0; i < Main.NUMBER_OF_VERTEX; i++)
        vertex[i] = 0;
}

StackItem::StackItem(const StackItem& orig) {
    this->path = new list(orig.path);
    vertex = orig.vertex;
    pathDeegre = orig.pathDeegre;
}

virtual StackItem::~StackItem() {

}

void StackItem::addEdge(Edge* e) {
    path->add(e);

    //pridal sem hranu, takze se mi zmenil stupen vrcholu
    //toto pridavani je, aby metoda countDegree() mela s cim porovnavat,
    //teda ona ma s cim co porovnavat, ale kdyz se setkaji dve hrany prvne, tak je stupen
    //dva, kdyz se k nim prida treti, tak uz se inkremetuje o jednicku a to v metode
    //countDegree() neslo udelat, tak je to obesle tady


    if (vertex[e.getStart()] == 0) {
        vertex[e.getStart()]++;
    }
    if (vertex[e.getEnd()] == 0) {
        vertex[e.getEnd()]++;
    }
}

bool StackItem::isContainsEdge(Edge* e) {
    return path->isIn(e);
}

int StackItem::pathSize(){
    return path->getSize();
}

Edge* StackItem::getFristEdge(){
    return path->getFirst();
}

Edge* StackItem::getLastEdge(){
    return path->getLast();
}

bool StackItem::isPathEmpty(){
    return path->isEmpty();
}