/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package kgmpar;

import java.util.Arrays;
import java.util.LinkedList;

/**
 *
 * @author Lukáš
 *
 * Nas vlastni objekt na reprezentaci jednoho prvku pomoci spojoveho seznamu
 *
 */
public class StackItem implements Cloneable {

    LinkedList<Edge> path;
    int pathDeegre;
    int[] vertex;

    StackItem(LinkedList<Edge> list) {
        path = list;//(LinkedList<Edge>) list.clone();;
        vertex = new int[Main.NUMBER_OF_VERTEX];
        //jelikoz z kazdeho vrcholu vychazi hrana, tak ma minimalne stupen 1
        Arrays.fill(vertex, 0);
    }

    StackItem(StackItem path/*,Edge e*/) {
        this.path = (LinkedList<Edge>) path.getPath().clone();
        //this.path = path.getPath();
        vertex = path.getVertexArray().clone();
        //this.addEdge(e);
    }



    public void addEdge(Edge e) {
        path.add(e);

        //pridal sem hranu, takze se mi zmenil stupen vrcholu
        //toto pridavani je, aby metoda countDegree() mela s cim porovnavat,
        //teda ona ma s cim co porovnavat, ale kdyz se setkaji dve hrany prvne, tak je stupen
        //dva, kdyz se k nim prida treti, tak uz se inkremetuje o jednicku a to v metode
        //countDegree() neslo udelat, tak je to obesle tady


        if (vertex[e.getStart()] == 0){
            vertex[e.getStart()]++;
        } if ( vertex[e.getEnd()] == 0) {
             vertex[e.getEnd()]++;
        }
        }

    public boolean isContainsEdge(Edge e) {
        return path.contains(e);
    }

    public int pathSize() {
        return path.size();
    }

    public Edge getLastEdge() {
        return path.getLast();
    }

    public Edge getFristEdge() {
        return path.getFirst();
    }

    public boolean isPathEmpty() {
        return path.isEmpty();
    }

    /**
     * Vypis hrany v textove podobe
     * @return nazevHrany(start,end)
     */
    @Override
    public String toString() {
        String s = "";
        for (int i = 0; i < path.size(); i++) {
            s = s + path.get(i).getName() + "(" + path.get(i).getStart() + "," + path.get(i).getEnd() + ")" + " ";
        }

        return s;
    }

    /**
     * Nefunguje korektne!
     * @return
     */
    @Override
    public Object clone() {
        try {
            return super.clone();
        } catch (CloneNotSupportedException e) {
            System.out.println("Cloning not allowed");
            ;
            return this;
        }
    }

    /*
     * maze danou hranu
     */
    void removeEdge(Edge edge) {
        path.remove(edge);
    }

    /*
     * maze posledni hranu
     */
    void removeLastEdge() {
        path.removeLast();
    }

    /*
     * vraci hranu
     */
    Edge getEdge(int l) {
        return path.get(l);
    }

    /*
     * Nepouzita metora
     */
    public void setVertexDegree(int vertex, int degree) {
        this.vertex[vertex] = degree;
    }

    /***
     * Metoda funguje nasledovne:
     * pri kazdem pridani hranu projdu danou cestu a pokud se souradnice pridane
     * hrany shoduji s jinymi souradnicemi, tak se graf v danem miste vetvi.
     *
     * uzel se muze vetvit 4 zpusoby
     *
     * zde je prostor pro optimalizaci - pri tom pruchodu zjistovat jake je tam maximalni
     * cislo a to ulozit do globalni promenne, ke ktere bude konstatni pristup, ted se
     * pri kazdem dotazu na stupen cesty prochazi cele pole a hleda se maximum
     * cz zabere linearni cas
     *
     * @return
     */
    public void countDegree() {
        
        Edge addedEdge = path.getLast();
      
        //promena, ve ktere ukladam navstivenou souradnici uzlu, aby se mi neduplikovali vyskyty
        //pri pocitani stupne vrcholu a mohl projit vsechny moznosti, kdyby se mi shodovala
        //druha souradnice
        int visitedVertexS = -1;
        int visitedVertexE = -1;
        for (int i = 0; i < path.size() - 1; i++) {
            //pokud nejaka hrana navazuje, tak u daneho vrcholu zvysime stupen
            if ((addedEdge.getStart() == path.get(i).getStart()) && visitedVertexS != addedEdge.getStart()) {
                vertex[addedEdge.getStart()]++;
                visitedVertexS = addedEdge.getStart();
            }
            if ((addedEdge.getStart() == path.get(i).getEnd())&& visitedVertexS != addedEdge.getStart()) {
                vertex[addedEdge.getStart()]++;
                visitedVertexS = addedEdge.getStart();
            }
            if (addedEdge.getEnd() == path.get(i).getStart() && visitedVertexE != addedEdge.getEnd()) {
                vertex[addedEdge.getEnd()]++;
                visitedVertexE = addedEdge.getEnd();
            }
            if (addedEdge.getEnd() == path.get(i).getEnd()&& visitedVertexE != addedEdge.getEnd()) {
                vertex[addedEdge.getEnd()]++;
                visitedVertexE = addedEdge.getEnd();
            }

        }


    }

    /**
     * Vybere nejvetsi stupen
     *
     * Zde je misto pro optimalizace s metedou countDegree()
     * @return
     */
    public int getMaxDegree() {
        //countDegree();
        int max = Integer.MIN_VALUE;
        for (int i = 0; i < vertex.length; i++) {

            if (vertex[i] > max) {
                max = vertex[i];
            }

        }
        return max;
    }

    /*
     * vraci cestu s hranami
     */
    public LinkedList<Edge> getPath() {
        return this.path;

    }

    /*
     * Vrati ti pole vrcholu
     */
    public int[] getVertexArray() {
        return this.vertex;
    }
    
    /**
     * ladici metoda
     * @return
     */
    String getVertexArrayToString() {
       String s = "";
       for(int i=0; i<vertex.length; i++){
        s = s + "["+vertex[i]+"]";
       }
       return s;
    }
}
