/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.unionfind;

/**
 *
 * @author root
 */
public class WeightedQuickUnion1 {
    private int id[];
    private int sz[];
    
    //.constructor for quick union
    public WeightedQuickUnion1(int N)
    {
        id=new int[N];
        sz=new int[N];
        for(int i=0;i<N;i++)
        {
          id[i]=i;  
          sz[i]=1;
        }
    }

    //finding the root node
    private int root(int i)
    {
        int root=i;
        while(root!=id[root])
        {
            root=id[root];
        }
        int k=id[i];
        
        while(i!=k)
        {
            k=id[i];
            id[i]=root;
            
        }
        
        return root;
    }
    
    //finding equivlance of elements a and b
    public boolean find(int a,int b)
    {
        return root(a)==root(b);
    }

        //merging two equivalence classes
    public void union(int a,int b)
    {
        int r1=root(a);
        int r2=root(b);
        if(sz[r1]<sz[r2])
        {
            id[r1]=r2;
            sz[r1]=sz[r1]+sz[r2];
        }
        else
        {
            id[r2]=r1;
            sz[r2]=sz[r1]+sz[r2];
            
        }
        
        
        
    }


}
