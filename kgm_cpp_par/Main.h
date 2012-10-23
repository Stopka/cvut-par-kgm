/* 
 * File:   Main.h
 * Author: stopka
 *
 * Created on 23. říjen 2012, 16:07
 */

#ifndef MAIN_H
#define	MAIN_H

#include "Stack.h"
#include "List.h"


class Main {
public:
    Stack* stack;
    //arraylist na uchovani hran
    List* edges;
    //pocet vrcholu grafu
    static int NUMBER_OF_VERTEX;
    
    Main();
    Main(const Main& orig);
    virtual ~Main();
    
    static void main();
    void loadEdges(int** adjMatrix);
    void DFS(int k);
private:
    //stupen kostry
    int minDegree;
    bool minDegreeInited;
    //hledana kostra s minimálním stupnem
    StackItem* minSpanningTree;
    bool isCycle(Edge* e, StackItem* path);
    bool isEdgeInPath(Edge* e, StackItem* path);
};

#endif	/* MAIN_H */

