# -*- coding: utf-8 -*-
"""
Created on Tue Jul 30 18:21:09 2013

@author: pi19404
"""
import sklearn.datasets as datasets
import numpy as np
import DecisionStump1
from sklearn import tree
import sys
import pickle
import StringIO, pydot 

iris = datasets.load_iris()
idx=0
XX=np.reshape(iris.data[:,idx],(-1,1))
y=iris.target
c=DecisionStump1.DecisionStumpLearner()
classifier=c.test(XX,y)      
yy1=c.predict_class(XX,classifier) 
print float(sum(y==yy1))/y.shape[0]

#print classifier.threshold,classifier.left,classifier.right

dtree = tree.DecisionTreeClassifier(criterion = "entropy",max_depth=1)
dtree = dtree.fit(XX,y)
yy=dtree.predict(XX)
print float(sum(y==yy))/y.shape[0]


dot_data = StringIO.StringIO() 
tree.export_graphviz(dtree, out_file=dot_data) 
graph = pydot.graph_from_dot_data(dot_data.getvalue()) 
graph.write_pdf("iris.pdf") 