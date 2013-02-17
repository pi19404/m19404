/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.unionfind;
/**
 *
 * @author root
 */
public class WeightedQuickUnion {
    private int id[];
    private int sz[];
    
    //.constructor for quick union
    public WeightedQuickUnion(int N)
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
        //System.out.println(":"+i+":"+id[i]);
        while(i!=id[i])
        {
          //  System.out.println(":"+i+":"+id[i]);
            i=id[i];
        }
        return i;
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
        //System.out.println("r1:"+r1+":"+r2+":"+sz[r1]+":"+sz[r2]);
        if (r1 == r2) return;
        
        if(sz[r1]<sz[r2])
        {
            id[r1]=r2;
            sz[r2]=sz[r1]+sz[r2];
        }
        else 
        {
            id[r2]=r1;
            sz[r1]=sz[r1]+sz[r2];
            
        }
        
        
        
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
    
 public static void main1(String[] args) {
        int N = StdIn.readInt();
        WeightedQuickUnionUF uf = new WeightedQuickUnionUF(N);

        // read in a sequence of pairs of integers (each in the range 0 to N-1),
         // calling find() for each pair: If the members of the pair are not already
        // call union() and print the pair.
        while (!StdIn.isEmpty()) {
            int p = StdIn.readInt();
            int q = StdIn.readInt();
            if (uf.connected(p, q)) continue;
            uf.union(p, q);
            StdOut.println(p + " " + q);
        }
        StdOut.println(uf.count() + " components");
    }

    public static void main(String[] args) {
        WeightedQuickUnion u=new WeightedQuickUnion(10);
        
        u.union(3,6);
        u.print();
        u.union(7,0);
        u.print();
        
        u.union(8,9);
        u.print();
        u.union(6,4);
        u.print();
        u.union(9,0);
        u.print();
        u.union(2,5);
        u.print();
        u.union(2,6);
        u.print();
        u.union(0,3);
        u.print();
        u.union(7,1);
        u.print();
        
        /*
        WeightedQuickUnionUF uf = new WeightedQuickUnionUF(10);
        uf.union(8,3);
        
        uf.union(7,2);
        
        
        uf.union(3,0);
        
        uf.union(9,1);
        
        uf.union(8,4);
        
        uf.union(0,5);
        
        uf.union(7,9);
        
        uf.union(2,0);
        
        uf.union(6,7);
        uf.print();
        */
    }
}
