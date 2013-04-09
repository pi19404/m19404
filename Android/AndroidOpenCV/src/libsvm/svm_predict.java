package libsvm;
import libsvm.*;
import java.io.*;
import java.util.*;

//import android.util.Log;

import com.android.main.Log;
import com.googlecode.javacpp.FloatPointer;


public class svm_predict {
	int predict_probability=1;
	svm_model model ;
	float feature[];
	public int svm_type;
	public int nr_class;
	public double[] prob_estimates=null;
	public int[] labels;
	public double outputp;
	public double v; //label
	
	   long end ;
	   long time ;
	   long start;
	   int first=0;
	   int m;
	   svm_node[] x ;
	private static double atof(String s)
	{
		return Double.valueOf(s).doubleValue();
	}

	private static int atoi(String s)
	{
		return Integer.parseInt(s);
	}
	
	private  double predict(FloatPointer input, svm_model model, int predict_probability) throws IOException
	{
	
                try
                {
		

		if(predict_probability == 1)
		{
			if(svm_type == svm_parameter.EPSILON_SVR ||
			   svm_type == svm_parameter.NU_SVR)
			{
				System.out.print("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma="+svm.svm_get_svr_probability(model)+"\n");
			}
			
		}
		if(input!=null)
		{

			if(first==0)
			{
			m=input.capacity();
			x = new svm_node[m];
			first=1;
			
			for(int j=0;j<m;j++)
			{
				x[j] = new svm_node();
				x[j].index = j+1;
				x[j].value = input.get(j);
				
			}
			
			}
			else
                        {
			for(int j=0;j<m;j++)
			{
				x[j].index = j+1;
				x[j].value = input.get(j);
				
			}
                        }
                         
			v=-1;
			outputp=0;
			if (predict_probability==1 && (svm_type==svm_parameter.C_SVC || svm_type==svm_parameter.NU_SVC))
			{
				
					
					 
				v = svm.svm_predict_probability(model,x,prob_estimates);
                                
				for(int j=0;j<nr_class;j++)
				{
					if(v==labels[j])
					{
						outputp=prob_estimates[j];
						//Log.e("EE","  :"+v+" FF : "+labels[j]+"  "+outputp);
					}
					else
					{
						//outputp=prob_estimates[j];
				//		Log.e("EE","  :"+v+" FF : "+labels[j]+" "+prob_estimates[j]);
					}
			
				}
				
			}
			else
			{
				
				 
				v = svm.svm_predict(model,x);
                                //Log.e("EE","AAA :"+v);
				
				
			}

//			if(v == target)
//				++correct;
//			error += (v-target)*(v-target);
//			sumv += v;
//			sumy += target;
//			sumvv += v*v;
//			sumyy += target*target;
//			sumvy += v*target;
//			++total;
			
		}
//		if(svm_type == svm_parameter.EPSILON_SVR ||
//		   svm_type == svm_parameter.NU_SVR)
//		{
//			System.out.print("Mean squared error = "+error/total+" (regression)\n");
//			System.out.print("Squared correlation coefficient = "+
//				 ((total*sumvy-sumv*sumy)*(total*sumvy-sumv*sumy))/
//				 ((total*sumvv-sumv*sumv)*(total*sumyy-sumy*sumy))+
//				 " (regression)\n");
//		}
//		else
//			System.out.print("Accuracy = "+(double)correct/total*100+
//				 "% ("+correct+"/"+total+") (classification)\n");
                }
                catch(Exception e)
                {
                    e.printStackTrace();
                    
                }
                return v;
	}

	private static void exit_with_help()
	{
		System.err.print("usage: svm_predict [options] test_file model_file output_file\n"
		+"options:\n"
		+"-b probability_estimates: whether to predict probability estimates, 0 or 1 (default 0); one-class SVM not supported yet\n");
		System.exit(1);
	}

	public double prediction(FloatPointer features) throws IOException
	{
		

		
		if(predict_probability == 1)
		{
			if(svm.svm_check_probability_model(model)==0)
			{
				Log.e("EEEE","Model does not support probabiliy estimates\n");predict_probability=0;
			}
		}
		else
		{
			if(svm.svm_check_probability_model(model)!=0)
			{
				Log.e("EEEE","Model supports probability estimates, but disabled in prediction.\n");
                                predict_probability=1;
			}
		}
		
		 
		start = System.currentTimeMillis();
		double ll=predict(features,model,predict_probability);
		end = System.currentTimeMillis();
		time = end - start;

                return ll;
		
	}
	
	public  void init(String argv[]) throws IOException
	{
		int i;

		// parse options
		for(i=0;i<argv.length;i++)
		{
			if(argv[i].charAt(0) != '-') break;
			++i;
			switch(argv[i-1].charAt(1))
			{
				case 'b':
					predict_probability = atoi(argv[i]);
					break;
				default:
					System.err.print("Unknown option: " + argv[i-1] + "\n");
					exit_with_help();
			}
		}
		if(i>=argv.length-2)
			exit_with_help();
		try 
		{
			BufferedReader input = new BufferedReader(new FileReader(argv[i]));
			
			
			
			
			input.close();
		
		} 
                catch(Exception e) 
                {
                    e.printStackTrace();
                }
	}
	
	public void load_model(String model_path) throws IOException
	{
            try{
		model = svm.svm_load_model(model_path);
		svm_type=svm.svm_get_svm_type(model);
		nr_class=svm.svm_get_nr_class(model);
		
		labels=new int[nr_class];
		svm.svm_get_labels(model,labels);
		prob_estimates = new double[nr_class];
            }
            catch(Exception e)
            {
                e.printStackTrace();
            }
                    
                    
	}
	
	public void set_probability()
	{
		predict_probability=1;
	}
	
	public void reset_probability()
	{
		predict_probability=0;
	}
	
}