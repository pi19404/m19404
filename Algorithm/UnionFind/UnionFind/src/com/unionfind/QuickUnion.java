/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.unionfind;

/**
 *
 * @author root
 */
public class QuickUnion {
    private int id[];
    
    //.constructor for quick union
    public QuickUnion(int N)
    {
        id=new int[N];
        for(int i=0;i<N;i++)
          id[i]=i;  
    }
    
    //finding the root node
    private int root(int i)
    {
        while(i!=id[i])
        {
            i=id[i];
        }
        return i;
    }
    
    //merging two equivalence classes
    public void merge(int a,int b)
    {
        int r1=root(a);
        int r2=root(b);
        id[r1]=r2;
        
    }
    
    //finding equivlance of elements a and b
    public boolean find(int a,int b)
    {
        return root(a)==root(b);
    }
    
    
    public static void main()
    {
        QuickUnion u=new QuickUnion(10);
    }
    
}
