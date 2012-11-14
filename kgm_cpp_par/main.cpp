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

/*
 * 
 */
int main(int argc, char** argv) {
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
    int pesek;
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
    int blbost = 0;
    while (processStatus != STATUS_FINISHED) {

        //optimalizacni vec, mozna ale zbytecna - rozmyslet
        if ((my_rank == 0) && (processorCount == 1) && (processStatus == STATUS_IDLE)) {
            processStatus = STATUS_FINISHED;
        }
        //TODO - dodelat pesky, neboli algoritmus pro distribuovane ukonceni vypoctu :P

        //chovani procesu pri vyckani
        if (processStatus == STATUS_IDLE) {
            //jsme IDLE a procesor 0 -> musime barvit a rozesla peska
            if (my_rank == 0) {
                color = WHITE_PROCESS;
                pesek = WHITE_PROCESS;
                MPI_Send(&pesek, 1, MPI_INT, 1, MSG_TOKEN, MPI_COMM_WORLD);
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
                        //TODO - prisla prace -> deserializovat data a praci si hodit na zasobnik 
                        MPI_Recv(data, maxDataLenght, MPI_INT, MPI_ANY_SOURCE, MSG_WORK_SENT, MPI_COMM_WORLD, &status);
                        StackItem* item = new StackItem(data, edges, NUMBER_OF_VERTEX);
                        stack->push(item);
                        processStatus = STATUS_WORKING;
                        cout << "[MPI_WORK_RECEIVED] Process:" << my_rank << "dostal praci" << endl;
                        cout << "Item:" << *item << endl << flush;
                        break;
                    }
                        /*Zamitava odpoved na zadost o práci*/
                    case MSG_WORK_NOWORK:
                        MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        processStatus = STATUS_IDLE;
                        cout << "[MPI_Recv NO?WORK] " << my_rank << endl;
                        break;
                        /*Prisel pesek zajistujici ukonceni vypoctu ADUV*/
                    case MSG_TOKEN:
                        cout << "[MPI_Recv] " << "Sender: " << status.MPI_SOURCE << " Target " << my_rank << " Message: " << status.MPI_TAG << endl;
                        MPI_Recv(&pesek, 1, MPI_INT, status.MPI_SOURCE, MSG_TOKEN, MPI_COMM_WORLD, &status);
                        if ((my_rank == 0) && (pesek == BLACK_PROCESS)) {
                            cout << "MASTER PRIJAL BLACK TOKEN" << endl;
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
                        //TODO dale oblouszit
                    case MSG_FINISH:
                        MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MSG_FINISH, MPI_COMM_WORLD, &status);
                        processStatus = STATUS_FINISHED;
                        cout << "[MPI_Recv] " << "Sender: " << status.MPI_SOURCE << " Target " << my_rank << " Message: " << status.MPI_TAG << endl;
                        break;
                    default:; //chyba("neznamy typ zpravy");
                        break;
                }
            }
            blbost++;
            if (blbost == 5) {
                processStatus = STATUS_FINISHED;
                break;
            }

        }/*End of IDLE*/
        int counter = 0;
        //chovani procesu pri praci
        if (processStatus == STATUS_WORKING) {
            while (!stack->is_empty() && processStatus != STATUS_FINISHED) {
                MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
                if (flag) {
                    //obdobne se musi obslouzit zpravy
                    switch (status.MPI_TAG) {
                            //prisla o zadost o praci
                        case MSG_WORK_REQUEST:
                            MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, MSG_WORK_REQUEST, MPI_COMM_WORLD, &status);
                            //nebudeme posilat praci, kdyz sam mam malo
                            cout << my_rank << ":Prisla zadost o praci od procesu cislo: " << status.MPI_SOURCE << endl;

                            if (stack->getSize() > 2) {
                                StackItem* item = stack->popLast();
                                cout << *item << endl;
                                data = item->serialize();
                                cout << my_rank << ":Prace odeslana" << endl;
                                MPI_Send(data, data[0] + 1, MPI_INT, status.MPI_SOURCE, MSG_WORK_SENT, MPI_COMM_WORLD);
                                //TODO resit pesky a barvicky
                            } else {
                                cout << my_rank << ":Sam mam malo prace" << endl;
                                //sorry neni prace :P
                                MPI_Send(0, 0, MPI_INT, status.MPI_SOURCE, MSG_WORK_NOWORK, MPI_COMM_WORLD);
                            }
                            break;
                    }

                }
                //a dale jiz pokracuje samotny vypocet....
                counter++;
                if (counter == 5) {
                    processStatus = STATUS_FINISHED;
                    break;
                };
            }
        }

    }
    MPI_Finalize();
    //Main* app = new Main(NUMBER_OF_VERTEX);
    //app->loadEdges(arr);

    //app->DFS(6);
    /*
    Main* app2 = new Main();
    app2->loadEdges(new int[][]{
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}});
    //app2.DFS(2);
     */
    return 0;
}