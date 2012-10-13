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

    StackItem(LinkedList<Edge> list){
        path = list;
        vertex = new int[Main.NUMBER_OF_VERTEX];
        //jelikoz z kazdeho vrcholu vychazi hrana, tak ma minimalne stupen 1
        Arrays.fill(vertex, 0);
    }

    public void addEdge(Edge e){
        path.add(e);

        //pridal sjem hranu, takze se mi zmenil stupen vrcholu

        if (vertex[e.getStart()] == 0 && vertex[e.getEnd()] == 0){
        vertex[e.getStart()]++;
        vertex[e.getEnd()]++;
        }
    }

    public boolean isContainsEdge(Edge e){
        return path.contains(e);
    }

    public int pathSize(){
        return path.size();
    }

    public Edge getLastEdge(){
        return path.getLast();
    }

    public Edge getFristEdge(){
        return path.getFirst();
    }

    public boolean isPathEmpty(){
        return path.isEmpty();
    }
    /**
     * Vypis hrany v textove podobe
     * @return nazevHrany(start,end)
     */
    @Override
    public String toString(){
        String s = "";
        for(int i=0; i<path.size();i++){
            s = s+path.get(i).getName()+"("+path.get(i).getStart()+","+path.get(i).getEnd()+")"+" ";
        }

        return s;
    }

    /**
     * Nefunguje korektne!
     * @return
     */
    @Override
    public Object clone(){
        try{
            return super.clone();
        }catch (CloneNotSupportedException e){
            System.out.println("Cloning not allowed");;
            return this;
        }
    }

    void removeEdge(Edge edge) {
        path.remove(edge);
    }

    void removeLastEdge(){
        path.removeLast();
    }

    Edge getEdge(int l) {
        return path.get(l);
    }



    public void setVertexDegree(int vertex, int degree){
        this.vertex[vertex] = degree;
    }

    /***
     * Metoda funguje nasledovne:
     * pri kazdem pridani hranu projdu danou cestu a pokud se souradnice pridane
     * hrany shoduji s jinymi souradnicemi, tak se graf v danem miste vetvi. Zbyva
     * vyresit, kde ukladat pocty vetveni? mozna pole vrcholu, kde klic bude vrchol
     * a hodnota stupen daneho vrcholu
     * @return
     */
    private void countDegree() {
        int degree = 0;

        Edge addedEdge = path.getLast();
 
        for(int i= 0; i < path.size()-1; i++){
            //pokud nejaka hrana navazuje, tak u daneho vrcholu zvysime stupen
            Edge compare = path.get(i);
            if ((addedEdge.getStart() == path.get(i).getStart())
                    ){

                vertex[addedEdge.getStart()]++;
                return;
            }else if ((addedEdge.getStart() == path.get(i).getEnd())
                    ){
                 vertex[addedEdge.getStart()]++;
                 return;
            }else if (addedEdge.getEnd() == path.get(i).getStart()){
                vertex[addedEdge.getEnd()]++;
                return;
            }else if (addedEdge.getEnd() == path.get(i).getEnd()){
                vertex[addedEdge.getEnd()]++;
                return;
            }

            /*else if(addedEdge.getEnd() == path.get(i).getEnd()
                    && addedEdge != path.get(i)){
                    vertex[addedEdge.getEnd()]++;
                    System.out.println("n?");
            }*/


        }

        
    }


    public int getMaxDegree(){
        countDegree();
        int max = Integer.MIN_VALUE;
        for(int i=0; i < vertex.length; i++){

            if (vertex[i] > max) max = vertex[i];

        }
        return max;
    }

}
