/* 
 * File:   Edge.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 16:21
 */

#ifndef EDGE_H
#define	EDGE_H
#include <iostream>
using namespace std;

class Edge {
    friend ostream& operator<<(ostream& os, const Edge& e);
public:
    Edge(const Edge& orig);
    Edge(int start, int end, int id = 0);
    virtual ~Edge();
    int getEnd() const;
    void setEnd(int end);
    int getId() const;
    void setId(int id);
    int getStart() const;
    void setStart(int start);
    bool connectsVertex(int v);
    bool operator==(const Edge& e) const;
    bool operator<(const Edge& e) const;
    bool operator>(const Edge& e) const;

private:
    int end;
    int start;
    int id;
};

#endif	/* EDGE_H */

