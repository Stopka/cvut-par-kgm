/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package kgmpar;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Stack;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Lukáš
 */
public class Main {

    //zasobnik s vlastnim objektem
    Stack<StackItem> stack = new Stack();
    //arraylist na uchovani hran
    ArrayList<Edge> edges = new ArrayList();
    //pocet vrcholu grafu
    static int NUMBER_OF_VERTEX = 6;

    //pruchod do hloubky respektive do k hloubky, kde k je pocet hran kostry
    private void DFS(int k) {
        //vlozim vsechny hrany na zásobník
        for (Edge e : edges) {
            //vytvorim zaznam (linked list)
            StackItem path = new StackItem(new LinkedList<Edge>());
            //vlozim hranu
            path.addEdge(e);

            //musim nastavit u vrcholu, ktera hrana spojuje stupen na 1
            //path.setVertexDegree(e.getStart(), 1);
            //path.setVertexDegree(e.getEnd(), 1);
            //a poslu na zasobnik
            stack.push(path);
        }

        int degreeTemp = -1;

        //zacnu procházet cely prostor
        while (!stack.isEmpty()) {
            //vemu si cestu neboli vraceni se o uroven zpet
            StackItem path = stack.pop();

            //projdu vsechny mozne cesty v grafu
            for (Edge e : edges) {
                //pokud bude hrana v ceste, neboli uz jsme hranu navstivili, tak nas hrana nezajima
                if (isEdgeInPath(e, path)) {
                    continue;
                }
                //pokud pridana hrana nevytvori cyklus
                if (!isCycle(e, path)) {
                    //netvori => muzeme hranu pridat do cesty
                    path.addEdge(e);

                    degreeTemp = path.getMaxDegree();

                    System.out.println("Kontrolni vypis");
                    System.out.println("Cesta: " + path.toString());
                    System.out.println("stupne: " + degreeTemp);
                    System.out.println("----------------------");

                    //a celou cestu hodime na zasobnik
                    stack.push(path);
                } else {
                    //tvori cyklus
                    // Todo - oriznou vetec, nejspise by mel stacit return z for cyklu
                }

                //spocitat proztimni stupen kostry/cestsy
            }
            //pokud mame k hran v ceste, meli bychom mit kostru
            if (path.pathSize() == k) {
                System.out.println("Kostra " + path.toString());
                System.out.println("Stupen: " + degreeTemp);
            }
        }
    }

    /**
     * Metoda, ktera matici sousednosti prevede na kolekci obsahujici hrany
     *
     * !!! Metoda nebude fungovat pro vetsi pocet hran, jelikoz kazde hrane
     * prirazuje jeji nazev az do pismenka "L", takze ve finalni verzi toto
     * odstranit (zatim slouzi k debugovani)
     *
     * @param adjMatrix - matice sousednosti
     */
    public void loadEdges(int[][] adjMatrix) {
        String[] alphabet = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"};
        int k = 0;
        //projdu celou matici
        for (int i = 0; i < adjMatrix.length; i++) {
            for (int j = 0; j < adjMatrix[i].length; j++) {
                //pokud narazim na hranu
                if (adjMatrix[i][j] == 1) {
                    //tak ji vytvorim
                    edges.add(new Edge(i, j, alphabet[k]));
                    //a vymazu druhy vyskyt, abych nemel duplicity
                    adjMatrix[i][j] = 0;
                    adjMatrix[j][i] = 0;
                    k++;

                }
            }
        }
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Main app = new Main();


        /***
         * Pokud budes zkouset jine grafy, tak se musi upravit konstanta na pocet vrcholu!!!!
         * a samozrejme parametr k u metody DFS
         */
        app.loadEdges(new int[][]{{0, 1, 0, 0, 1, 0},
                    {1, 0, 1, 0, 1, 0},
                    {0, 1, 0, 1, 0, 0},
                    {0, 0, 1, 0, 1, 1},
                    {1, 1, 0, 1, 0, 0},
                    {0, 0, 0, 1, 0, 0}});

        app.DFS(5);

        Main app2 = new Main();
        app2.loadEdges(new int[][]{{0, 1, 1},
                    {1, 0, 1},
                    {1, 1, 0}});
        //app2.DFS(2);
    }

    /**
     * Kontroluje zda zadana hrana e je obsazena v ceste
     * @param e kontrolovana hrana
     * @param path zadana cesta
     * @return true pokud se nachazi jinak false
     */
    private boolean isEdgeInPath(Edge e, StackItem path) {
        return path.isContainsEdge(e);
    }

    /**
     * Metoda na kontrolovani cyklu v zadane ceste pomoci koponent souvislosti
     * Na zacatku mame N komponenet souvislosti, kde N je pocet vrcholu. Kazdym
     * pridanim dalsi hrany do kostry se snizi pocet komponent souvislosti o 1.
     * Nove pridana hrana totiz musi spojovat dvojici vrchollu z dosud ruznych
     * ruznych koponenet souvislosti, jinak jeji zarazei vytvori cyklus.
     *
     * @param e pridavana hrana
     * @param path nase doposud kostra
     * @return true pokud tvori cyklus jinak false
     */
    private boolean isCycle(Edge e, StackItem path) {

        int[] komponent = new int[NUMBER_OF_VERTEX];
        //pocatecni jednoprvkove komponenty souvislosti
        for (int i = 0; i < komponent.length; i++) {
            komponent[i] = i;
        }

        //hranu pridame na konec seznamu do cesty
        path.addEdge(e);
        //pomocna promenna do cyklus
        int l = 0;

        //while (!copyPath.isPathEmpty()) {
        while (path.pathSize() != l) {
            //vezmu si hranu
            Edge edge = path.getEdge(l);
            int k1 = komponent[edge.getStart()];
            int k2 = komponent[edge.getEnd()];
            //pokud hrana patri do komponent souvislosti, tak mam cyklus
            if (k1 == k2) {
                //odstranim testovanou hranu
                path.removeLastEdge();
                return true;
            } else {
                //jinak musim poupravit komponentu souvislosti
                //respektive snizit jeji pocet 1
                for (int k = 0; k < komponent.length; k++) {
                    if (komponent[k] == k2) {
                        komponent[k] = k1;
                    }

                }
            }
            l++;

        }
        //zkoumanou hranu odstanim - v ceste byla docasne, kvuli kontrole

        path.removeLastEdge();
        return false;
    }
}
