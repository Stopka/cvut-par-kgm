/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package kgmpar;

/**
 *
 * @author Lukáš
 */
class Edge {

    private int end;
    private int start;
    private String name;

    Edge(int i, int j, String name) {
        start = i;
        end = j;
        this.name = name;
    }

    public String getName(){
        return this.name;
    }

    public int getEnd() {
        return end;
    }

    public void setEnd(int end) {
        this.end = end;
    }

    public int getStart() {
        return start;
    }

    public void setStart(int start) {
        this.start = start;
    }

    @Override
    public String toString(){
        return getName()+"("+getStart()+","+getEnd()+")";
    }

}
