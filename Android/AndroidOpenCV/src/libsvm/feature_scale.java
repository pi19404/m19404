/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package libsvm;


import com.android.main.Log;
import com.googlecode.javacpp.FloatPointer;
import com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;
import com.googlecode.javacv.cpp.opencv_imgproc.*;
import java.lang.Math.*;
import java.nio.FloatBuffer;
import java.io.*;
import java.util.*;
import java.text.DecimalFormat;
/**
 *
 * @author pi19404
 */
public class feature_scale {
   String restore_filename ;
    
   //FloatPointer feature_max;
   //FloatPointer feature_min;
        private String line = null;
        private double lower = -1.0;
	private double upper = 1.0;
	private double y_lower;
	private double y_upper;
	private boolean y_scaling = false;
	private double[] feature_max;
	private double[] feature_min;
	private double y_max = -Double.MAX_VALUE;
	private double y_min = Double.MAX_VALUE;
	private int max_index;
	private long num_nonzeros = 0;
	private long new_num_nonzeros = 0;
        BufferedReader fp_restore;
        
       private BufferedReader rewind(BufferedReader fp, String filename) throws IOException
	{
		fp.close();
		return new BufferedReader(new FileReader(filename));
	}
 	private double output_target(double value)
	{
		if(y_scaling)
		{
			if(value == y_min)
				value = y_lower;
			else if(value == y_max)
				value = y_upper;
			else
				value = y_lower + (y_upper-y_lower) *
				(value-y_min) / (y_max-y_min);
		}

		return value;
	}
        	private double output(int index, double value)
	{
		/* skip single-valued attribute */
		if(feature_max[index] == feature_min[index])
			return 0;

		if(value == feature_min[index])
			value = lower;
		else if(value == feature_max[index])
			value = upper;
		else
			value = lower + (upper-lower) * 
				(value-feature_min[index])/
				(feature_max[index]-feature_min[index]);

		if(value != 0)
		{
			//System.out.print(index + ":" + value + " ");
			new_num_nonzeros++;
                        return value;
		}
                else
                    return 0;
	}
                
        private String readline(BufferedReader fp) throws IOException
	{
		line = fp.readLine();
		return line;
	}

                        
   public feature_scale(String name) throws Exception
   {
       restore_filename=name;
       int i=0;
       num_nonzeros=0;
        if(restore_filename != null)
       {           
           max_index = 0;  
                int idx, c;
        try {
	     fp_restore = new BufferedReader(new FileReader(restore_filename));
	}
        catch (Exception e) {
            Log.e("EE","can't open file " + restore_filename);
            throw e;
	}
        Log.e("EE","open file " + restore_filename);
        if((c = fp_restore.read()) == 'y')
			{
				fp_restore.readLine();
				fp_restore.readLine();		
				fp_restore.readLine();		
			}
        fp_restore.readLine();
	fp_restore.readLine();
                        

String restore_line = null;
while((restore_line = fp_restore.readLine())!=null)
{
	StringTokenizer st2 = new StringTokenizer(restore_line);
	idx = Integer.parseInt(st2.nextToken());
	max_index = Math.max(max_index, idx);
			}
fp_restore = rewind(fp_restore, restore_filename);
                        


try {
			feature_max = new double[(max_index+1)];
			feature_min = new double[(max_index+1)];
		} catch(OutOfMemoryError e) {
			System.err.println("can't allocate enough memory");
			System.exit(1);
		}


 for(i=0;i<=max_index;i++)
		{
			feature_max[i] = -Double.MAX_VALUE;
			feature_min[i] = Double.MAX_VALUE;
		}

 		{
			// fp_restore rewinded in finding max_index 
			
			double fmin, fmax;

			fp_restore.mark(2);				// for reset
			if((c = fp_restore.read()) == 'y')
			{
				fp_restore.readLine();		// pass the '\n' after 'y'
				StringTokenizer st = new StringTokenizer(fp_restore.readLine());
				y_lower = Double.parseDouble(st.nextToken());
				y_upper = Double.parseDouble(st.nextToken());
				st = new StringTokenizer(fp_restore.readLine());
				y_min = Double.parseDouble(st.nextToken());
				y_max = Double.parseDouble(st.nextToken());
				y_scaling = true;
			}
			else
				fp_restore.reset();

			if(fp_restore.read() == 'x') {
				fp_restore.readLine();		// pass the '\n' after 'x'
				StringTokenizer st = new StringTokenizer(fp_restore.readLine());
				lower = Double.parseDouble(st.nextToken());
				upper = Double.parseDouble(st.nextToken());
				restore_line = null;
				while((restore_line = fp_restore.readLine())!=null)
				{
					StringTokenizer st2 = new StringTokenizer(restore_line);
					idx = Integer.parseInt(st2.nextToken());
					fmin = Double.parseDouble(st2.nextToken());
					fmax = Double.parseDouble(st2.nextToken());
					if (idx <= max_index)
					{
						feature_min[idx] = fmin;
						feature_max[idx] = fmax;
					}
				}
			}
			fp_restore.close();
		}
 
       }
        else{
        	Log.e("FFF","file name is null");
        }
    }
        
        
   public void svm_scale(FloatPointer descriptors) throws Exception
   {
       
                int i,index;
		BufferedReader fp = null;
		String save_filename = null;
		
		String data_filename = null;
                        	
		/* pass 3: scale */
		for(i=0;i<descriptors.capacity();i++)
		{
			int next_index = 1;
			double target;
			float value;


				index = i+1;
				value = descriptors.get(i);				
				value=(float)output(index, value);
				descriptors.put(i,value);
				//next_index = index + 1;



			
		}
               
   }
}
