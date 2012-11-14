/* 
 * File:   Edge.cpp
 * Author: stopka
 * 
 * Created on 21. říjen 2012, 14:35
 */

#include "Edge.h"

Edge::Edge(const Edge& orig) {
    this->start = orig.start;
    this->end = orig.end;
    this->id = orig.id;
}

Edge::Edge(int start, int end, int id) {
    this->start = start;
    this->end = end;
    this->id = id;
}

Edge::~Edge() {
}

int Edge::getEnd() const {
    return end;
}

void Edge::setEnd(int end) {
    this->end = end;
}

int Edge::getId() const {
    return id;
}

void Edge::setId(int id) {
    this->id = id;
}

int Edge::getStart() const {
    return start;
}

void Edge::setStart(int start) {
    this->start = start;
}

bool Edge::operator==(const Edge& e) const{
    if((start==e.start&&end==e.end)||(end==e.start&&start==e.end)){
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os,const Edge& e){
    os<<"(";
    //os<<e.id<<"|";
    if(e.start<e.end){
        os<<e.start<<","<<e.end;
    }else{
        os<<e.end<<","<<e.start;
    }
    os<<")";
    return os;
}

bool Edge::operator>(const Edge& e) const{
    if(this->id<e.id){
        return true;
    }
    return false;
}

bool Edge::operator<(const Edge& e) const{
    if(this->id>e.id){
        return true;
    }
    return false;
}