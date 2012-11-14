/* 
 * File:   main.cpp
 * Author: stopka
 *
 * Created on 21. říjen 2012, 14:30
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
//#include <mpi/mpi.h>
#include "Main.h"

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
    int NUMBER_OF_VERTEX=0;
    ifstream file;
    string line;
    file.open(file_name);
    file>>NUMBER_OF_VERTEX;
    cout<<NUMBER_OF_VERTEX<<endl;
    getline(file, line);
    int** arr = new int*[NUMBER_OF_VERTEX];
    for(int row=0;row<NUMBER_OF_VERTEX;row++){
        arr[row] = new int[NUMBER_OF_VERTEX];
        getline(file, line);
        for(int col=0;col<NUMBER_OF_VERTEX;col++){
            arr[row][col]=line[col]-'0';
            cout<<arr[row][col];
        }
        cout<<endl;
    }
    int dfsK=NUMBER_OF_VERTEX-1;
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