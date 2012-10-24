/* 
 * File:   Main.cpp
 * Author: stopka
 * 
 * Created on 23. říjen 2012, 16:07
 */

#include "Main.h"
#include "StackItem.h"

Main::Main(int VERTEX_NUM) {
    this->NUMBER_OF_VERTEX=VERTEX_NUM;
    stack=new Stack();
    edges=new List();
    minDegreeInited=false;
    minSpanningTree=NULL;
}

Main::Main(const Main& orig) {
}

Main::~Main() {
}

void Main::DFS(int k) {
    //vlozim vsechny hrany na zásobník
    for (int i=0;i<edges->getSize();i++) {
        Edge* e=edges->getItem(i);
        //vytvorim zaznam (linked list)
        StackItem* path = new StackItem(new List(),NUMBER_OF_VERTEX);
        //vlozim hranu
        path->addEdge(e);
        
        //Vypis
        cout<<"INIT ";
        //a cestu v grafu vlozim na zasobnik
        stack->push(path);
    }

    //zacnu procházet cely prostor
    while (!stack->is_empty()) {
        //vemu si cestu neboli vraceni se o uroven zpet
        StackItem* path = stack->pop();
        //pomocna cesta
        StackItem* aaa = 0;

        //projdu vsechny mozne cesty v grafu
        for (int it=0;it<edges->getSize(); it++) {
            Edge* e=edges->getItem(it);
            //pokud bude hrana v ceste, neboli uz jsme hranu navstivili, tak nas hrana nezajima
            if (isEdgeInPath(e, path)) {
                continue;
            }
            //pokud pridana hrana nevytvori cyklus
            if (!isCycle(e, path)) {

                // vytvorim novou cestou, kterou pozdeji ulozim na zasobnik
                //predam ji v konstruktoru dosavadni cestu
                aaa = new StackItem(*path);
                //pridam zkoumanou hranu do cesty
                aaa->addEdge(e);
                //spocitam jeji stupen s novou hranou
                aaa->countDegree();

                //zkoumam zda dana cesta nema vetsi stupen nez dosavadni nalezeny
                if (minDegreeInited && aaa->getMaxDegree() > minDegree) {
                    //pokud ano, tak orezavam :)
                    continue;
                }

                //pokud mam narok na kostru
                if (aaa->pathSize() == k) {

                    //zapamatuji si kostru
                    if (!minDegreeInited || aaa->getMaxDegree() < minDegree) {
                        minDegree = aaa->getMaxDegree();
                        minSpanningTree = aaa;
                        minDegreeInited = true;
                    }
                    //Vypis
                    //cout<<"Kostra: "<<(*aaa)<<" |"<<aaa->getMaxDegree()<<endl;
                    //dal nemusim prohledavat strom => orezavan
                    continue;
                    //ne break, jelikoz ten zahodi celou hladinu
                } else {
                    //nemam kostru tak pokracuji
                    //Vypis
                    
                    //a celou cestu hodime na zasobnik
                    stack->push(aaa);
                }
            } else {
                //tvori cyklus
                //System.out.println("Vetev: " + path.toString() + " s hranou: " + e.toString() + " tvori cyklus -> cut");
                //tato vetev, dal nevedete, ale porad mohou existovat jine moznosti v jinych vetvich
                //ktere musime dat na zasobnik
                continue;

            }
        }

    }

    cout << "Result" << endl << "--------------------" << endl;
    cout << "Kostra: " << (*minSpanningTree) << endl;
    cout << "Stupen: " << minSpanningTree->getMaxDegree();
}

void Main::loadEdges(int** adjMatrix) {
    int k = 0;
    //projdu celou matici
    for (int i = 0; i < Main::NUMBER_OF_VERTEX; i++) {
        for (int j = 0; j < Main::NUMBER_OF_VERTEX; j++) {
            //pokud narazim na hranu
            if (adjMatrix[i][j] == 1) {
                //tak ji vytvorim
                edges->add(new Edge(i, j, k));
                //a vymazu druhy vyskyt, abych nemel duplicity
                adjMatrix[i][j] = 0;
                adjMatrix[j][i] = 0;
                k++;

            }
        }
    }
}

bool Main::isEdgeInPath(Edge* e, StackItem* path) {
    return path->isContainsEdge(e);
}

bool Main::isCycle(Edge* e, StackItem* path) {

    int komponent[NUMBER_OF_VERTEX];
    //pocatecni jednoprvkove komponenty souvislosti
    for (int i = 0; i < NUMBER_OF_VERTEX; i++) {
        komponent[i] = i;
    }

    //hranu pridame na konec seznamu do cesty
    path->addEdge(e);
    //pomocna promenna do cyklus
    int l = 0;

    //while (!copyPath.isPathEmpty()) {
    while (path->pathSize() != l) {
        //vezmu si hranu
        Edge* edge = path->getEdge(l);
        int k1 = komponent[edge->getStart()];
        int k2 = komponent[edge->getEnd()];
        //pokud hrana patri do komponent souvislosti, tak mam cyklus
        if (k1 == k2) {
            //odstranim testovanou hranu
            path->removeLastEdge();
            return true;
        } else {
            //jinak musim poupravit komponentu souvislosti
            //respektive snizit jeji pocet 1
            for (int k = 0; k < NUMBER_OF_VERTEX; k++) {
                if (komponent[k] == k2) {
                    komponent[k] = k1;
                }

            }
        }
        l++;

    }
    //zkoumanou hranu odstanim - v ceste byla docasne, kvuli kontrole

    path->removeLastEdge();
    return false;
}

int Main::getNumOfVertex(){
    return this->NUMBER_OF_VERTEX;
}