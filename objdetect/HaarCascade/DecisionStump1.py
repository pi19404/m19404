# -*- coding: utf-8 -*-
"""
Created on Sat Jul 20 11:56:20 2013

@author: pi19404
"""

from sklearn import datasets
import numpy as np
import numpy as np
from numpy import zeros as np_zeros
from numpy import ones as np_ones
from numpy import bool as np_bool
from numpy import float32 as np_float32
from numpy import float64 as np_float64
import math
import cmath
from numpy import log as log
#from libc.math import log, pow
DTYPE = np_float32
DOUBLE = np_float64

#data structure to hold basic information
#for decision tree stump classification
class DecisionStump:
    def init(self,threshold,left,right):
        #split point
        self.threshold=threshold
        #class accociated with left and right tree
        self.left=left
        self.right=right
        
        
        
class DecisionStumpLearner:
    
    #initialization function
    def init(self,X,y,sample_mask):
        print X.shape
        self.n_samples, self.n_features_ = X.shape

        self.classes = unique_y = np.unique(y)
        self.classes=np.array(self.classes,'i')
        #self.classes=np.reshape(self.classes,(-1,1))
        #print self.classes
        self.n_classes = unique_y.shape[0]
        
        
        self.n_total_samples=self.n_samples
        self.n_left=0
        self.n_right=0
        self.lable_countl=np.zeros((self.n_classes))
        self.lable_countr=np.zeros((self.n_classes))
        self.lable_count_init=np.zeros((self.n_classes))
        
        #self.classes = np.array(self.classes,'i')
        
        
        if sample_mask == None :
            sample_mask=np.ones((self.n_samples))
        
        for j in xrange(self.n_total_samples):
            #skip samples not set in the mask array
            if sample_mask[j]==0:
                continue
            c=y[j]
            self.lable_count_init[c]+=1

        self.n_right=0
        k=0
        #set the initial counts
        for k in xrange(self.n_classes):            
            self.n_right+=self.lable_count_init[k]
            
        
        
        self.reset()
        self.sample_mask=sample_mask
        
    #reset the parameters
    def reset(self):
        self.n_left=0
        self.n_right=0
        for k in range(self.n_classes):            
            self.n_right+=self.lable_count_init[k]            
            self.lable_countl[k]=0
            self.lable_countr[k]=self.lable_count_init[k]
            
    #free allocates arrays
    def free(self):
        if self.sample_mask is not None :
            self.sample_mask=None

    #update the counts for indexes in range (a,b)
    def update_index(self,a,b,X,y,s_x,n_total_samples,sample_mask):
        n_classes=self.n_classes
        n_left=self.n_left
        n_right=self.n_right
        lable_countl=self.lable_countl
        lable_countr=self.lable_countr
        y_idx=0

        for idx in range(a,b):
            j=s_x[idx]
            
            if sample_mask[j]==0:
			continue
            
            y_idx=int(y[j])	
            
            n_left+=1	
            n_right-=1	
            lable_countl[y_idx]+=1
            lable_countr[y_idx]-=1
            
        
        self.n_left=n_left
        self.n_right=n_right
        self.lable_countr=lable_countr
        self.lable_countl=lable_countl
        
        #if all samples are exhausted
        if n_left <=0 or n_right <=0 :
		return False
		
	  #prevent any class from having 0 or negative count 
       #need this for entropy calculation
        for c in range(0,n_classes):
		if(lable_countl[c] <=0 or lable_countr[c] <=0):
               #   print lable_countr[c],lable_countl[c]
                  return False
	
	return True

        
    #find the next unique valid sample
    def find_next_sample(self,sample_index,X,s_x):
        if sample_index >-1:
            threshold=X[s_x[sample_index]]
            
        for idx in range( sample_index,self.n_total_samples):
            j=s_x[idx]		

            if X[j] > threshold+ 1.e-7:
			return idx
              
        return -1



#we will use entropy as criteria to decide which is the best split
#---------------------------------------------
#function which compute the entropy of data
#based on current split point
    def eval(self):
#array contaning the number of point belonging
#to class 0 or 1 to left of split point
        lable_countl=self.lable_countl
#array contaning the number of point belonging
#to class 0 or 1 to the right of split poin        
        lable_countr=self.lable_countr
#total number of points to left of split point                
        n_left=self.n_left
#total number of points to right of split point        
        n_right=self.n_right
#number of classes        
        n_classes=self.n_classes
        n_samples=self.n_samples
#average Entropy of points to left/right of split point
        H_left=0
        H_right=0
#for all the classes	 
        l1=self.lable_countl.argmax()
        l2=self.lable_countr.argmax()
        s1=sum(self.lable_countl)
        s2=sum(self.lable_countr)
        l11=-(lable_countl[l1]/s1)* log(lable_countl[l1]/s1) 
        l12=-(lable_countl[l2]/s2)* log(lable_countr[l2]/s2)
        l11=((s1/self.n_total_samples)*l11)+((s2/self.n_total_samples)*l12)
        
        
        for c in xrange(n_classes):
            #average entropy of points to left of split point 	      
            if lable_countl[c]>0:
                H_left -= ((lable_countl[c]/n_left) * log(lable_countl[c]/n_left))
#average entropy of points to right of split point 	                      
            if lable_countr[c]>0:
                H_right -= ((lable_countr[c]/n_right) * log(lable_countr[c]/n_right))

#weighing the left and right entopy by number of samples                            
        e1=float(n_left/n_samples)*H_left;
        e2=float(n_right/n_samples)*H_right;
#computing the total average entropy        
        total=e1+e2
        #print ":",total,H_left,H_right,e1+e2,n_right,n_left,lable_countl[0],lable_countl[1],lable_countl[2],lable_countr[0],lable_countr[1],lable_countr[2]
        #n_left,H_right,n_right,n_samples,l11        
        return (total,H_left,H_right)

    #fit decision Stump to training data
    def fit(self,X,y,sample_mask=None):
        
        
        X = np.asarray(X)       
        y = np.asarray(y,'i')   

        if X.dtype != np_float32 :
            X = np.asarray(X, dtype=DTYPE, order="F")

        if y.dtype != DOUBLE or not y.flags.contiguous:
            y = np.asarray(y, dtype=DOUBLE, order="C")

        y = np.asarray(y,'i')        
        y=np.reshape(y,(-1,1))
        
        unique_y= np.unique(y)
        self.miny=unique_y.min()

        y = np.searchsorted(unique_y, y) 
#class labels start from 0    
#initalize the decision tree stump classifier
#this method will initalize the variables required
#for training and prediction        
        self.init(X,y,sample_mask)  #initialization

        


        sample_mask=self.sample_mask
        best_error= float("inf")
        best_t=float("inf")
        
        
#sorting the input attribute values and 
#storing sorted index in arrays_x                
        s_x=np.argsort(X.T).astype(np.int32).T  #sort attributes in ascending order
        #print s_x
        a=0
        while True:
            #finding the next unique sample
            b=self.find_next_sample(a,X,s_x) #find the next sample
            if b==-1:
                break
                
            #updating the indexes
            if not self.update_index(a,b,X,y,s_x,self.n_total_samples,sample_mask) :
                a=b
                continue
		
            #evalucating the entrpy
            (error,H_left,H_right)=self.eval()

            if error < best_error:
            #check if error is best error
                X_a=X[s_x[a]]
                X_b=X[s_x[b]]
#set the split as mid point between adjancet indexed values
                t=X_a+((X_b-X_a)/2)
            #print error,H_left,H_right,t            
                                                
                if t==X_b:
                    t=X_a
#update the split point                     
                best_t=t
                
                best_error=error            
                
            a=b



            
        self.best_t=best_t
        self.best_error=best_error

        self.reset()

        #compute the class lables based on the best split
        a=0
        b=best_t
        for idx in range (0,self.n_total_samples):
            j=s_x[idx]
            if sample_mask[j]==0:
			continue

            if X[j] > self.best_t:
                break

            y_idx=int(y[j])	            
            self.lable_countl[y_idx]+=1
            self.lable_countr[y_idx]-=1
            
        
        self.left=self.lable_countl.argmax()
        self.right=self.lable_countr.argmax()
        #s1=sum(self.lable_countl)
        #s2=sum(self.lable_countr)
        
        #for c in xrange(0,3):
        #    l1=-(self.lable_countl[c]/s1)*log(self.lable_countl[c]/s1)        
        #    l2=-(self.lable_countr[c]/s2)*log(self.lable_countr[c]/s2)
        
        self.left=self.lable_countl.argmax()+self.miny
        self.right=self.lable_countr.argmax()+self.miny        
        #print "total",s1,s2,l1,l2,(((s1/self.n_total_samples)*l1)+((s2/self.n_total_samples)*l2))
        self.free()
        
        #create a decision tump and store information
        classifier=DecisionStump()
        classifier.init(float(best_t),int(self.left),int(self.right))
        self.threshold=self.best_t
        return classifier
        
    #perform prediction
    def predict(self,X):
        n_samples = X.shape[0]        
        y = np.zeros((n_samples))
        for i in range(0,n_samples):
            #apply the decision rule
            if X[i] <= self.threshold:
                y[i]=self.left
            else:
                y[i]=self.right
                
        return y    

    #perform prediction
    def predict_class(self,X,classifier=None):
        n_samples = X.shape[0]

        if classifier is None:
            classifier=self
            
        y = np.zeros((n_samples))        
        for i in range(0,n_samples):
            if X[i] <= classifier.threshold:
                y[i]=classifier.left
            else:
                y[i]=classifier.right
                
        return y    
        
    def test(self,X,Y):
        #iris = datasets.load_iris()       
        #XX=np.reshape(iris.data[:,0],(-1,1)).shape
        classifier=self.fit(X,Y)        
        return classifier
        
        
     
           