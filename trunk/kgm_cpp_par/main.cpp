/* 
 * File:   main.cpp
 * Author: stopka
 *
 * Created on 21. říjen 2012, 14:30
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mpi/mpi.h>
#include "Main.h"

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

using namespace std;
const char* file_name = "graph.txt";

/*
 * 
 */
int main(int argc, char** argv) {
    /*
    int myrank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    printf("Hello from processor %d of %d\n", myrank, nprocs);

    MPI_Finalize();
     */
    int NUMBER_OF_VERTEX = 0;
    ifstream file;
    string line;
    file.open(file_name);
    file >> NUMBER_OF_VERTEX;
    cout << NUMBER_OF_VERTEX << endl;
    getline(file, line);
    int** arr = new int*[NUMBER_OF_VERTEX];
    for (int row = 0; row < NUMBER_OF_VERTEX; row++) {
        arr[row] = new int[NUMBER_OF_VERTEX];
        getline(file, line);
        for (int col = 0; col < NUMBER_OF_VERTEX; col++) {
            arr[row][col] = line[col] - '0';
            cout << arr[row][col];
        }
        cout << endl;
    }
    int dfsK = NUMBER_OF_VERTEX - 1;


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
    int flag;
    int processToAskForWork;
    /*
    int NUMBER_OF_VERTEX = 6;
    int dfsK = 5;
    int** arr = new int*[NUMBER_OF_VERTEX];
    arr[0] = new int[NUMBER_OF_VERTEX]{0, 1, 0, 0, 1, 0};
    arr[1] = new int[NUMBER_OF_VERTEX]{1, 0, 1, 0, 1, 0};
    arr[2] = new int[NUMBER_OF_VERTEX]{0, 1, 0, 1, 0, 0};
    arr[3] = new int[NUMBER_OF_VERTEX]{0, 0, 1, 0, 1, 1};
    arr[4] = new int[NUMBER_OF_VERTEX]{1, 1, 0, 1, 0, 0};
    arr[5] = new int[NUMBER_OF_VERTEX]{0, 0, 0, 1, 0, 0};
    /*//*
    int NUMBER_OF_VERTEX=3;
    int** arr = new int*[NUMBER_OF_VERTEX];
    arr[0] = new int[NUMBER_OF_VERTEX]{0, 1, 1};
    arr[1] = new int[NUMBER_OF_VERTEX]{1, 0, 1};
    arr[2] = new int[NUMBER_OF_VERTEX]{1, 1, 0};
    /**/
    /***
     * Pokud budes zkouset jine grafy, tak se musi upravit konstanta na pocet vrcholu!!!!
     * a samozrejme parametr k u metody DFS
     */



    /* Inicializace MPI prostředí */
    MPI_Init(&argc, &argv);
    /* Zjistí jaký jsem proces a uloží do my_rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /*Zjistí kolik procesoru se ucastni vypoctu*/
    MPI_Comm_size(MPI_COMM_WORLD, &processorCount);

    /*procesy musi cekat, nez se rozdeli data*/
    MPI_Barrier(MPI_COMM_WORLD);
    cout << "MPI_Barrier START" << endl;

    /*nastavim casovac - pro pocitani celkoveho casu programu*/
    if (my_rank == 0) {
        time1 = MPI_Wtime();
    }
    /*Hodit vsechna hrany na zasobnik a vsechny nebo co nejvice to pujdu rozeslu procesorum. Pro ty, pro ktere nic nemam
     jim poslu NOWORK znacku*/

    /*praci rozdeluje procesor 0 - master pro tuto chvili*/
    if (my_rank == 0) {
        cout << "zacinam rozesilat" << endl;

        //od 1, jelikoz procesor 0 rozdeluje praci
        for (int i = 1; i < processorCount; i++) {
            //TODO z edges vemu hranu a posli ji na zpracovani procesoru
            /*Pokud je co brát*/
            if (true){
                Edge* e; /*= edges->getItem(i)*/
                //TODO praci poslu procesoru i
                //sendFirstWork(i, e);
                cout << "Poslal jsem praci procesu " << i << endl; 
            //neni prace    
            }else{
                MPI_Send(0, 0, MPI_INT, i, MSG_WORK_NOWORK, MPI_COMM_WORLD);
                cout << "Poslal jsem NO_WORK procesu " << i << endl;
            }
        }
        //na stack daneho procesoru si ulozit praci a zbytek ulozit na "master stack"
        
       processStatus = STATUS_WORKING;

    } else {
        /*Nejsem hlavni proces, co rozdeluje prace => bud praci dostanu nebo ne*/
        while (processStatus == 0) {
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
            if (flag) {

                switch (status.MPI_TAG) {
                        /*Prisla zadost o praci, jsem IDLE, posilam tedy NO_WORK*/
                    case MSG_WORK_NOWORK:
                        MPI_Recv(0, 0, MPI_INT, 0, MSG_WORK_NOWORK, MPI_COMM_WORLD, &status);
                        processStatus = STATUS_IDLE;
                        break;
                    case MSG_WORK_SENT:
                        //receiveFirstWork(&status,myStack)
                        processStatus = STATUS_WORKING;
                        break;
                }
            }
        }

    }

    /*Nastaveni koho se ptat na praci - aby nedoslo zbytecne k hromadnemu ptani se procesu 0*/
    processToAskForWork = (my_rank + 1) % processorCount;

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "MPI_Barrier INIT WORK SENT" << endl;
    /*KONEC INICIALIZACE*/




    Main* app = new Main(NUMBER_OF_VERTEX);
    app->loadEdges(arr);

    app->DFS(dfsK);
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