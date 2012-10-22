/* 
 * File:   Edge.h
 * Author: stopka
 *
 * Created on 21. říjen 2012, 16:21
 */

#ifndef EDGE_H
#define	EDGE_H

class Edge {
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
    bool operator==(const Edge& e) const;

private:
    int end;
    int start;
    int id;
};

#endif	/* EDGE_H */

