/* 
 * File:   main.cpp
 * Author: stopka
 *
 * Created on 21. říjen 2012, 14:30
 */

#include <cstdlib>

#include "Main.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Main* app = new Main();

    int** arr = new int*[Main::NUMBER_OF_VERTEX];
    arr[0]=new int[Main::NUMBER_OF_VERTEX]{0, 1, 0, 0, 1, 0};
    arr[1]=new int[Main::NUMBER_OF_VERTEX]{ 1, 0, 1, 0, 1, 0};
    arr[2]=new int[Main::NUMBER_OF_VERTEX]{ 0, 1, 0, 1, 0, 0};
    arr[3]=new int[Main::NUMBER_OF_VERTEX]{ 0, 0, 1, 0, 1, 1};
    arr[4]=new int[Main::NUMBER_OF_VERTEX]{ 1, 1, 0, 1, 0, 0};
    arr[5]=new int[Main::NUMBER_OF_VERTEX]{ 0, 0, 0, 1, 0, 0};
    /***
     * Pokud budes zkouset jine grafy, tak se musi upravit konstanta na pocet vrcholu!!!!
     * a samozrejme parametr k u metody DFS
     */
    app->loadEdges(arr);

    app->DFS(5);
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

