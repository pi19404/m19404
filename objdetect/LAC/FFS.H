void TrainWeakClassifiers(void);
void LoadWeakClassifiers(int* const indexes);
void ForwardFeatureSelection(AdaBoostClassifier& ada,const int round,const bool update,const int method);