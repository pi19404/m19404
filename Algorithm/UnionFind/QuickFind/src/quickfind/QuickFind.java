/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package quickfind;

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
        for (int i = 0; i < id.length; i++)
        if (id[i] == pid) id[i] = qid;
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
    }
}
