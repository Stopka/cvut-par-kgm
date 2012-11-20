/* 
 * File:   StackItem.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 15:04
 */
#include "StackItem.h"
#include "Stack.h"
#include "Main.h"

StackItem::StackItem(List* list, int NUMBER_OF_VERTEX) {
    this->NUMBER_OF_VERTEX = NUMBER_OF_VERTEX;
    path = list; //(list<Edge>) list.clone();
    vertex = new int[NUMBER_OF_VERTEX];
    //jelikoz z kazdeho vrcholu vychazi hrana, tak ma minimalne stupen 1
    countDegree();
}

StackItem::StackItem(const StackItem& orig) {
    this->NUMBER_OF_VERTEX = orig.NUMBER_OF_VERTEX;
    this->path = new List(*orig.path);
    vertex = new int[NUMBER_OF_VERTEX];
    for (int i = 0; i < NUMBER_OF_VERTEX; i++) {
        vertex[i] = orig.vertex[i];
    }
    pathDeegre = orig.pathDeegre;
}

StackItem::~StackItem() {
    delete path;
    delete[] vertex;
}

void StackItem::addEdge(Edge* e) {
    path->add(e);
    vertex[e->getStart()]++;
    vertex[e->getEnd()]++;
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
    countDegree();
}

void StackItem::removeLastEdge() {
    path->removeLast();
    countDegree();
}

Edge* StackItem::getEdge(int index) {
    return path->getItem(index);
}

void StackItem::countDegree() {
    //projedu vrcholy a kouknu, kolik hran vede do tohoto vrcholu
    for (int i = 0; i < NUMBER_OF_VERTEX; i++) {
        vertex[i] = 0;
    }
    Edge* e;
    for (int i = 0; i< this->path->getSize(); i++) {
        e = this->path->getItem(i);
        vertex[e->getStart()]++;
        vertex[e->getEnd()]++;
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

bool StackItem::operator<(const Edge& e) {
    if (*(getLastEdge()) < e) {
        return true;
    }
    return false;
}

int* StackItem::serialize() {
    int* serMatrix = new int[path->getSize() + 1];
    serMatrix[0] = path->getSize();
    for (int i = 0; i < path->getSize(); i++) {
        serMatrix[i + 1] = path->getItem(i)->getId();
    }
    return serMatrix;
}

StackItem::StackItem(int* id_array, List* edges, int NUMBER_OF_VERTEX) {
    this->NUMBER_OF_VERTEX = NUMBER_OF_VERTEX;
    this->path = new List();
    this->vertex = new int[NUMBER_OF_VERTEX];
    for (int i = 1; i < (id_array[0] + 1); i++) {
        this->addEdge(edges->findById(id_array[i]));
        this->countDegree();
    }
}
