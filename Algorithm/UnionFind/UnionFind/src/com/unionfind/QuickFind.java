/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.unionfind;

/**
 *
 * @author root
 */
public class QuickFind {

    private int id[];
    
    
    //constructor for QuickFind Class
    public QuickFind(int N)
    {
        //creating a array
        id= new int[N];
        //initialising the equivalence classes
        for(int i=0;i<N;i++)
            id[i]=i;
    }
    
    //method to check if p and q belong to same equivalence class
    public boolean find(int p,int q)
    {
        return id[p] == id[q];
    }
 
    //merge the equivalence classes of p and q and assign the label of class q
    //to class p
    public void union(int p, int q)
    {
        int pid = id[p];
        int qid = id[q];
        if(pid==qid)
            return;
        for (int i = 0; i < id.length; i++)
        if (id[i] == pid) id[i] = qid;
    }
    
    public void print(){
        String s=new String();
        for(int i=0;i<id.length;i++)
            s=s+" "+id[i];
        System.out.println(s); 
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        QuickFind u=new QuickFind(10);
        u.union(6,9);
        u.union(3,5);
        u.union(1,5);
        u.union(4,9);
        u.union(0,3);        
        u.union(7,1);
        u.print();
    }
}
