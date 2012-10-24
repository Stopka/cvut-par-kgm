/* 
 * File:   StackItem.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 15:04
 */

#include "StackItem.h"
#include "Stack.h"
#include "Main.h"

StackItem::StackItem(List* list,int NUMBER_OF_VERTEX) {
    this->NUMBER_OF_VERTEX=NUMBER_OF_VERTEX;
    path = list; //(list<Edge>) list.clone();
    vertex = new int[NUMBER_OF_VERTEX];
    //jelikoz z kazdeho vrcholu vychazi hrana, tak ma minimalne stupen 1
    for (int i = 0; i < NUMBER_OF_VERTEX; i++)
        vertex[i] = 0;
}

StackItem::StackItem(const StackItem& orig) {
    this->NUMBER_OF_VERTEX=orig.NUMBER_OF_VERTEX;
    this->path = new List(*orig.path);
    vertex = orig.vertex;
    pathDeegre = orig.pathDeegre;
}

StackItem::~StackItem() {

}

void StackItem::addEdge(Edge* e) {
    path->add(e);

    //pridal sem hranu, takze se mi zmenil stupen vrcholu
    //toto pridavani je, aby metoda countDegree() mela s cim porovnavat,
    //teda ona ma s cim co porovnavat, ale kdyz se setkaji dve hrany prvne, tak je stupen
    //dva, kdyz se k nim prida treti, tak uz se inkremetuje o jednicku a to v metode
    //countDegree() neslo udelat, tak je to obesle tady


    if (vertex[e->getStart()] == 0) {
        vertex[e->getStart()]++;
    }
    if (vertex[e->getEnd()] == 0) {
        vertex[e->getEnd()]++;
    }
}

bool StackItem::isContainsEdge(Edge* e) {
    return path->isIn(e);
}

int StackItem::pathSize() {
    return path->getSize();
}

Edge* StackItem::getFristEdge() {
    return path->getFirst();
}

Edge* StackItem::getLastEdge() {
    return path->getLast();
}

bool StackItem::isPathEmpty() {
    return path->isEmpty();
}

ostream& operator<<(ostream& os, const StackItem& s) {
    os << *(s.path);
    return os;
}

void StackItem::removeEdge(Edge* edge) {
    path->remove(edge);
}

void StackItem::removeLastEdge() {
    path->removeLast();
}

Edge* StackItem::getEdge(int index) {
    return path->getItem(index);
}

void StackItem::countDegree() {
    Edge* addedEdge = path->getLast();
    //promena, ve ktere ukladam navstivenou souradnici uzlu, aby se mi neduplikovali vyskyty
    //pri pocitani stupne vrcholu a mohl projit vsechny moznosti, kdyby se mi shodovala
    //druha souradnice
    int visitedVertexS = -1;
    int visitedVertexE = -1;
    for (int i = 0; i < path->getSize() - 1; i++) {
        //pokud nejaka hrana navazuje, tak u daneho vrcholu zvysime stupen
        if ((addedEdge->getStart() == path->getItem(i)->getStart()) && visitedVertexS != addedEdge->getStart()) {
            vertex[addedEdge->getStart()]++;
            visitedVertexS = addedEdge->getStart();
        }
        if ((addedEdge->getStart() == path->getItem(i)->getEnd()) && visitedVertexS != addedEdge->getStart()) {
            vertex[addedEdge->getStart()]++;
            visitedVertexS = addedEdge->getStart();
        }
        if (addedEdge->getEnd() == path->getItem(i)->getStart() && visitedVertexE != addedEdge->getEnd()) {
            vertex[addedEdge->getEnd()]++;
            visitedVertexE = addedEdge->getEnd();
        }
        if (addedEdge->getEnd() == path->getItem(i)->getEnd() && visitedVertexE != addedEdge->getEnd()) {
            vertex[addedEdge->getEnd()]++;
            visitedVertexE = addedEdge->getEnd();
        }

    }
}

int StackItem::getMaxDegree() {
    //countDegree();
    int max = 0;
    for (int i = 0; i < NUMBER_OF_VERTEX; i++) {
        if (vertex[i] > max) {
            max = vertex[i];
        }
    }
    return max;
}

List* StackItem::getPath() {
    return this->path;
}

int* StackItem::getVertexArray() {
    return this->vertex;
}