/* 
 * File:   main.cpp
 * Author: stopka, Lukas
 *
 * Created on 21. říjen 2012, 14:30
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mpi/mpi.h>
#include "Stack.h"
#include "StackItem.h"
#include "List.h"
//#include "Main.h"

#define CHECK_MSG_AMOUNT  100
#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004
#define MSG_NEW_MINIMUM  1005

#define STATUS_WORKING  2001
#define STATUS_IDLE     2002
#define STATUS_FINISHED 2999

#define WHITE_PROCESS 3001
#define BLACK_PROCESS 3002

#define WHITE_TOKEN 3003
#define BLACK_TOKEN 3004

using namespace std;
const char* file_name = "graph.txt";
Stack* stack = new Stack();
List* edges;
int NUMBER_OF_VERTEX = 0;

bool isEdgeInPath(Edge* e, StackItem* path) {
    return path->isContainsEdge(e);
}

bool isCycle(Edge* e, StackItem* path) {

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

/*
 * 
 */
int main(int argc, char** argv) {
    /*Promenne*/
    /*Zda byla poprve nalezen nejaky stupen kostry*/
    bool minDegreeInited = false;
    /*Prozatimni kostra s nejmensim stupnem*/
    StackItem* minSpanningTree = NULL;
    /*Nejmensi nalezeny stupen kostry*/
    int minDegree;
    /*Spodni mez stupne kostry - 2*/
    int lowestPossibleDegree = 2;

    /*Inicializace MPI promennych*/
    /*Cislo procesu - 0 je master*/
    int my_rank;
    /*Pocet procesu*/
    int processorCount;
    /*mereni casu */
    double time1, time2 = 0;
    /*Aktualni stav procesu*/
    int processStatus = 0;
    MPI_Status status;
    /*Priznak MPI zprav*/
    int flag;
    /*Proces, ktery je pozadan o praci*/
    int processToAskForWork;
    /*velikost posilanych dat TODO*/
    int maxDataLenght = 200;
    /*Posílaná data*/
    int* data = new int[maxDataLenght];
    /*pešek*/
    int pesek = 0;
    /*pomocna promena, ktera si pamatuje, zda byl odeslan pesek*/
    bool wasPesekSent = false;
    /*barva procesu*/
    int color;
    /*Pomocna promena, aby se nezadalo o praci vicekrat*/
    bool wasRequestedForWork = false;



    /* Inicializace MPI prostředí */
    MPI_Init(&argc, &argv);
    /* Zjistí jaký jsem proces a uloží do my_rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /*Zjistí kolik procesoru se ucastni vypoctu*/
    MPI_Comm_size(MPI_COMM_WORLD, &processorCount);

    /*procesy musi cekat, nez se rozdeli data*/
    MPI_Barrier(MPI_COMM_WORLD);
    cout << "MPI_Barrier START" << endl;

    ifstream file;
    string line;
    file.open(file_name);
    file >> NUMBER_OF_VERTEX;
    //cout << NUMBER_OF_VERTEX << endl;
    getline(file, line);
    int** adjMatrix = new int*[NUMBER_OF_VERTEX];
    for (int row = 0; row < NUMBER_OF_VERTEX; row++) {
        adjMatrix[row] = new int[NUMBER_OF_VERTEX];
        getline(file, line);
        for (int col = 0; col < NUMBER_OF_VERTEX; col++) {
            adjMatrix[row][col] = line[col] - '0';
            //          cout << arr[row][col];
        }
        //    cout << endl;
    }
    int dfsK = NUMBER_OF_VERTEX - 1;


    edges = new List();
    int k = 0;
    //projdu celou matici
    for (int i = 0; i < NUMBER_OF_VERTEX; i++) {
        for (int j = 0; j < NUMBER_OF_VERTEX; j++) {
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



    /*nastavim casovac - pro pocitani celkoveho casu programu*/
    if (my_rank == 0) {
        time1 = MPI_Wtime();
    }

    /*praci rozdeluje procesor 0 - master pro tuto chvili*/
    if (my_rank == 0) {

        /*prvotni naplneni zasobniku*/
        for (int i = 0; i < edges->getSize(); i++) {
            Edge* e = edges->getItem(i);
            //vytvorim zaznam (linked list)
            StackItem* path = new StackItem(new List(), NUMBER_OF_VERTEX);
            //vlozim hranu
            path->addEdge(e);

            //Vypis
            //cout<<"INIT ";
            //a cestu v grafu vlozim na zasobnik
            stack->push(path);
        }
        processStatus = STATUS_WORKING;

    } else {
        //na zacatku nemam praci
        processStatus = STATUS_IDLE;
    }

    /*obarvime proces na bilou*/
    color = WHITE_PROCESS;
    /*Nastaveni koho se ptat na praci - aby nedoslo zbytecne k hromadnemu ptani se procesu 0*/
    processToAskForWork = (my_rank + 1) % processorCount;

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "MPI_Barrier END" << endl;
    /*KONEC INICIALIZACE*/

    int counter = 0;
    while (processStatus != STATUS_FINISHED) {

        //optimalizacni vec, mozna ale zbytecna - rozmyslet
        if ((my_rank == 0) && (processorCount == 1) && (processStatus == STATUS_IDLE)) {
            processStatus = STATUS_FINISHED;
        }

        //chovani procesu pri vyckani
        if (processStatus == STATUS_IDLE) {
            //ADUV bod 1
            if ((my_rank == 0) && (!wasPesekSent)) {
                color = WHITE_PROCESS;
                pesek = WHITE_PROCESS;
                MPI_Send(&pesek, 1, MPI_INT, 1, MSG_TOKEN, MPI_COMM_WORLD);
                wasPesekSent = true;
                //ADUV bod 3
            } else if ((pesek != 0) && (my_rank != 0)) {
                int next_rank = (my_rank + 1) % processorCount;
                MPI_Send(&pesek, 1, MPI_INT, next_rank, MSG_TOKEN, MPI_COMM_WORLD);
                color = WHITE_PROCESS;
                pesek = 0;
            }

            //jelikoz jsem IDLE, tak bych mel pozadat o praci
            int target = processToAskForWork % processorCount;
            if (!wasRequestedForWork) {
                if (target != my_rank) {
                    cout << "Zadost o praci: " << my_rank << " zada proces " << target << " o praci" << endl;
                    MPI_Send(0, 0, MPI_INT, target, MSG_WORK_REQUEST, MPI_COMM_WORLD);
                    wasRequestedForWork = true;
                }
            }
            processToAskForWork++;

            //i v IDLE stavu musim obslouzit prichozi zpravy - zejmena peska a prichozi praci
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

            if (flag) {
                switch (status.MPI_TAG) {
                        /*Prisla zadost o praci, jsem IDLE, posilam tedy NO_WORK*/
                    case MSG_WORK_REQUEST:
                        MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        MPI_Send(0, 0, MPI_INT, status.MPI_SOURCE, MSG_WORK_NOWORK, MPI_COMM_WORLD);
                        cout << "[MPI_Recv] " << "Sender: " << status.MPI_SOURCE << " Target " << my_rank << " Message: " << status.MPI_TAG << endl;
                        break;
                        /*Prisla prace*/
                    case MSG_WORK_SENT:
                    {
                        //prijmu data
                        MPI_Recv(data, maxDataLenght, MPI_INT, MPI_ANY_SOURCE, MSG_WORK_SENT, MPI_COMM_WORLD, &status);
                        //deseralizuji a vytvorim prvek na zasbonik
                        StackItem* item = new StackItem(data, edges, NUMBER_OF_VERTEX);
                        //vlozim na zasobnik a proces muze zacit pracovat
                        stack->push(item);
                        processStatus = STATUS_WORKING;
                        cout << "[MPI_WORK_RECEIVED] Process:" << my_rank << "dostal praci" << endl;
                        cout << my_rank << ":Item:" << *item << endl << flush;
                        wasRequestedForWork = false;
                        break;
                    }
                        /*Zamitava odpoved na zadost o práci*/
                    case MSG_WORK_NOWORK:
                        MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        processStatus = STATUS_IDLE;
                        cout << "[MPI_Recv NO?WORK] " << my_rank << endl;
                        wasRequestedForWork = false;
                        break;
                        /*Prisel pesek zajistujici ukonceni vypoctu ADUV*/
                    case MSG_TOKEN:
                        /*Jestliže Pi obdrží peška, pak má-li Pi barvu B, nastaví barvu peška na B. 
                         * Jakmile se Pi stane idle, pošle peška po kružnici procesoru Pi+1 a nastaví svoji barvu na W. 
                         */
                        cout << "[MPI_Recv] " << "Sender: " << status.MPI_SOURCE << " Target " << my_rank << " Message: " << status.MPI_TAG << endl;
                        MPI_Recv(&pesek, 1, MPI_INT, status.MPI_SOURCE, MSG_TOKEN, MPI_COMM_WORLD, &status);

                        if ((my_rank == 0) && (pesek == BLACK_PROCESS)) {
                            cout << "MASTER PRIJAL BLACK TOKEN" << endl;
                            wasPesekSent = false;
                        }

                        if ((my_rank == 0) && (pesek == WHITE_PROCESS)) {
                            processStatus = STATUS_FINISHED;
                            cout << "MAIN PROCESS STATUS_FINISHED" << endl;
                        } else {
                            if ((color == BLACK_PROCESS)) {
                                pesek = BLACK_PROCESS;
                            }
                        }
                        break;
                        /*Prislo nove minimum*/
                    case MSG_NEW_MINIMUM:
                        int message;
                        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MSG_NEW_MINIMUM, MPI_COMM_WORLD, &status);
                        //prijmul jsem spodni mez - nic nemuze byt lepsi -> end
                        if (message == lowestPossibleDegree) {
                            processStatus = STATUS_FINISHED;
                        } else if (message < minDegree) {
                            minDegree = message;
                        }
                        break;
                    case MSG_FINISH:
                        MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MSG_FINISH, MPI_COMM_WORLD, &status);
                        processStatus = STATUS_FINISHED;
                        cout << "[MPI_Recv] " << "Sender: " << status.MPI_SOURCE << " Target " << my_rank << " Message: " << status.MPI_TAG << endl;
                        break;
                    default:; //chyba("neznamy typ zpravy");
                        break;
                }
            }


        }/*End of IDLE*/
        //chovani procesu pri praci
        if (processStatus == STATUS_WORKING) {
            while (!stack->is_empty()) {
                MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
                if (flag) {
                    //obdobne se musi obslouzit zpravy
                    switch (status.MPI_TAG) {
                            //prisla o zadost o praci
                        case MSG_WORK_REQUEST:
                            MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MSG_WORK_REQUEST, MPI_COMM_WORLD, &status);
                            //nebudeme posilat praci, kdyz sam mam malo
                            cout << my_rank << ":Prisla zadost o praci od procesu cislo: " << status.MPI_SOURCE << endl;
                            //TODO tohle taky rozmyslet, zda by to optimalizacne slo udelat lepe
                            if (stack->getSize() > 2) {
                                StackItem* item = stack->popLast();
                                //cout << *item << endl;
                                data = item->serialize();
                                cout << my_rank << ":Prace odeslana" << endl;
                                MPI_Send(data, data[0] + 1, MPI_INT, status.MPI_SOURCE, MSG_WORK_SENT, MPI_COMM_WORLD);

                                //ADUV Jestliže procesor Pi pošle práci procesoru Pj, kde i>j, pak Pi nastaví svou barvu na B. 
                                if (my_rank > status.MPI_SOURCE) {
                                    color = BLACK_PROCESS;
                                    if (pesek != 0) {
                                        pesek = BLACK_PROCESS;
                                    }
                                }

                            } else {
                                cout << my_rank << ":Sam mam malo prace" << endl;
                                //sorry neni prace :P
                                MPI_Send(0, 0, MPI_INT, status.MPI_SOURCE, MSG_WORK_NOWORK, MPI_COMM_WORLD);
                            }
                            break;
                        case MSG_NEW_MINIMUM:
                            int message;
                            MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MSG_NEW_MINIMUM, MPI_COMM_WORLD, &status);
                            //prijmul jsem spodni mez - nic nemuze byt lepsi -> end
                            if (message == lowestPossibleDegree) {
                                processStatus = STATUS_FINISHED;
                            } else if (message < minDegree) {
                                minDegree = message;
                            }
                            break;
                        case MSG_FINISH:
                            break;
                        case MSG_TOKEN:
                            MPI_Recv(&pesek, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TOKEN, MPI_COMM_WORLD, &status);
                            if (my_rank == 0) {
                                wasPesekSent = false;
                            }

                            break;
                    }

                }
                /*DFS Start*/
                //vemu si cestu neboli vraceni se o uroven zpet
                StackItem* path = stack->pop();
                //pomocna cesta
                StackItem* aaa = 0;
                //projdu vsechny mozne cesty v grafu
                for (int it = 0; it < edges->getSize(); it++) {
                    Edge* e = edges->getItem(it);
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
                            delete(aaa);
                            aaa = NULL;
                            continue;
                        }
                        //pokud mam narok na kostru
                        if (aaa->pathSize() == dfsK) {
                            //pokud je menšího stupně zapamatuji si
                            if (!minDegreeInited || aaa->getMaxDegree() < minDegree) {
                                minDegree = aaa->getMaxDegree();
                                if (minSpanningTree != NULL) {
                                    delete minSpanningTree;
                                    minSpanningTree = NULL;
                                }
                                if (minDegree == lowestPossibleDegree) {
                                    processStatus = STATUS_FINISHED;
                                }
                                minSpanningTree = aaa;
                                cout << "Procesor " << my_rank << " nasel kostru" << *aaa << endl;
                                cout << "Stupne" << minDegree << endl;
                                minDegreeInited = true;
                                //Jelikoz jsem nasel nejlepsi minimum, tak ho odeslu vsem procesorum
                                for (int i = 0; i < processorCount; i++) {
                                    if (i != my_rank) {
                                        //cout << "Odesilam New minimum " << minDegree << " do " << i << endl;
                                        MPI_Send(&minDegree, 1, MPI_INT, i, MSG_NEW_MINIMUM, MPI_COMM_WORLD);
                                    }
                                }
                            } else {
                                delete aaa;
                                aaa = NULL;
                            }
                            //dal nemusim prohledavat strom => orezavan
                            continue;
                            //ne break, jelikoz ten zahodi celou hladinu
                        } else {
                            //nemam kostru tak pokracuji
                            //a celou cestu hodime na zasobnik
                            stack->push(aaa);
                        }
                    }
                }
                delete path;
                path = NULL;
               
                /*DFS End*/

                //a dale jiz pokracuje samotny vypocet....
                /*cout << my_rank << ":Procesor cislo " << my_rank << " pracuje :)" << endl;
                cout << my_rank << "Zbyva prvku na zasobniku: " << stack->getSize() << endl;*/

                //stack->pop();
                /*counter++;
                if (counter == 5) {
                    processStatus = STATUS_FINISHED;
                    break;
                }*/
            }//end of while
            //TODO rozmyslet !!!
            /*Pokud predchazejici cyklus skonci, tak to znamena, ze je prazdny zasobnik*/
            if (stack->is_empty()) {
                processStatus = STATUS_IDLE;
                cout << my_rank << ":zasobnik is empty" << endl;
            }
        }//end of main while

    }


    MPI_Barrier(MPI_COMM_WORLD);
    //cout << "MPI_Barrier END" << endl;
    cout << "Procesor cislo " << my_rank << "se dostal az sem" << endl;
    if (my_rank == 0) {
        time2 = MPI_Wtime();

        double totalTime = time2 - time1;

        cout << "Celkovy cas vypoctu: " << totalTime << endl;
        printf("Spotrebovany cas je %f.\n", totalTime);
    }

    //TODO vypis nejlepsi kostry a stupne

    MPI_Finalize();
    return 0;
}
