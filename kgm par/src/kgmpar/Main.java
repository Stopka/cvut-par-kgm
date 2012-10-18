/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package kgmpar;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Stack;

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
    //stupen kostry
    private int minDegree = Integer.MAX_VALUE;
    //hledana kostra s minimálním stupnem
    private StackItem minSpanningTree = null;

    //pruchod do hloubky respektive do k hloubky, kde k je pocet hran kostry
    private void DFS(int k) {
        //vlozim vsechny hrany na zásobník
        for (Edge e : edges) {
            //vytvorim zaznam (linked list)
            StackItem path = new StackItem(new LinkedList<Edge>());
            //vlozim hranu
            path.addEdge(e);
            //a cestu v grafu vlozim na zasobnik
            stack.push(path);
        }

        //zacnu procházet cely prostor
        while (!stack.isEmpty()) {
            //vemu si cestu neboli vraceni se o uroven zpet
            StackItem path = stack.pop();
            //kontrolni vypis stromu
            //System.out.println("POP: " + path.toString() + " " + path.getMaxDegree());

            //pomocna cesta
            StackItem aaa = null;

            //projdu vsechny mozne cesty v grafu
            for (Edge e : edges) {
                //pokud bude hrana v ceste, neboli uz jsme hranu navstivili, tak nas hrana nezajima
                if (isEdgeInPath(e, path)) {
                    continue;
                }
                //pokud pridana hrana nevytvori cyklus
                if (!isCycle(e, path)) {

                    // vytvorim novou cestou, kterou pozdeji ulozim na zasobnik
                    //predam ji v konstruktoru dosavadni cestu
                    aaa = new StackItem(path);
                    //pridam zkoumanou hranu do cesty
                    aaa.addEdge(e);
                    //spocitam jeji stupen s novou hranou
                    aaa.countDegree();
                    
                    /**kontrolni vypis
                    System.out.println("Cesta: " + aaa);
                    System.out.println("Ddegree" + aaa.getVertexArrayToString());
                    */
                     
                    //zkoumam zda dana cesta nema vetsi stupen nez dosavadni nalezeny
                    if (aaa.getMaxDegree() > minDegree) {
                        //pokud ano, tak orezavam :)
                        //System.out.println("Cesta " + aaa + " ma vetsi stupen nez nejmensi nalezeny");
                        continue;
                    }

                    //pokud mam narok na kostru
                    if (aaa.pathSize() == k) {

                        //zapamatuji si kostru
                        if (aaa.getMaxDegree() < minDegree) {
                            minDegree = aaa.getMaxDegree();
                            minSpanningTree = aaa;
                        }
                        //kontrolni vypis
                        //System.out.println("Kostra " + aaa.toString() + " stupen: " + aaa.getMaxDegree());
                        //dal nemusim prohledavat strom => orezavan
                        continue;
                        //ne break, jelikoz ten zahodi celou hladinu
                    } else {
                        //nemam kostru tak pokracuji
                        //kontrolni vypis prochazeneho stromu
                        //System.out.println("Push: " + aaa.toString() + " " + aaa.getMaxDegree());

                        //a celou cestu hodime na zasobnik
                        stack.push(aaa);
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
       
        System.out.println("Result:");
        System.out.println("Kostra: " + minSpanningTree.toString() + "stupne: " + minSpanningTree.getMaxDegree());
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
     * U tehle metody je prostor pro optimalizaci, ale nebudeme tak pilni :D
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
