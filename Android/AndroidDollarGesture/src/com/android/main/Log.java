/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.android.main;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Log {
	
static Method m;	
static Object obj;
static int mode=0;

	public Log()
	{
		Class[] paramString = new Class[2];	
		paramString[0] = String.class;
		paramString[1] = String.class;
		
		String sClassName = "android.util.Log";
		try {
		    Class classToInvestigate = Class.forName(sClassName);
		    obj = classToInvestigate.newInstance();
		    m = classToInvestigate.getMethod("e",new Class[] { String.class,String.class });
		  //  System.out.println("DDDDDDDDdd");
		
		} catch (ClassNotFoundException e) {
		    m=null;
		    //System.out.println("DDDDDDDDdd 1");
		} catch (Exception e) {
		    // Unknown exception
		}
 
	}
	
	
	public static void e(String a,String b)
	{
		
		android.util.Log.e(a,b);
		return;
		/*
		if(mode==0)
		{
mode=1;
Class[] paramString = new Class[2];	
paramString[0] = String.class;
paramString[1] = String.class;

String sClassName = "android.util.Log";
try {
	
    Class classToInvestigate = Class.forName(sClassName);
    android.util.Log.e(a,b+":"+mode+":"+"1"+classToInvestigate.toString());
    obj = classToInvestigate.newInstance();
    android.util.Log.e(a,b+":"+mode+":"+"2");
    m = classToInvestigate.getMethod("e",paramString);
    android.util.Log.e(a,b+":"+mode+":"+"3");
    
    //System.out.println("DDDDDDDDdd");

} catch (ClassNotFoundException e) {
    m=null;
    //System.out.println("DDDDDDDDdd 1");
} catch (Exception e) {
    e.printStackTrace();
}

		}
		
		if(m!=null)
		{
		try {
			m.invoke(obj,a,b);
		} catch (IllegalAccessException e1) {
			
			e1.printStackTrace();
		} catch (IllegalArgumentException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (InvocationTargetException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		}
		else
		{
		//System.out.println("DDDDDDDDdd 2");
		System.err.println(a+","+b);
		}
		*/
	}
	
}
