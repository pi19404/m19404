#ifndef AdaBoostClassifier1
#define AdaBoostClassifier1
enum GOAL_METHODS {GOAL_METHOD_MIN_ERROR = 1, GOAL_METHOD_VALID_DR = 2, GOAL_METHOD_FIX_FP = 3};

struct AdaBoostClassifier
{
	int count;
	SimpleClassifier* scs;
	REAL* alphas;
	REAL thresh;

	AdaBoostClassifier();
	~AdaBoostClassifier();
	void Clear(void);
	AdaBoostClassifier& operator=(const AdaBoostClassifier& source);
	void InitToGivenSize(const int size);
	
	void WriteToFile(ofstream& f) const;
	void ReadFromFile(ifstream& f);

	inline REAL GetValue(const IntImage& im) const;
	inline int ApplyImagePatch(const IntImage& im) const;

	void AddOneSimpleClassifier(bool* used);
	void TrainAdaBoost(int round,const bool update,const int method);
	void TrainLDS(int round,const bool update,const int method);
};

REAL AdaBoostClassifier::GetValue(const IntImage& im) const
{
	int i;
	REAL value;

	value = 0.0;
	for(i=0;i<count;i++) value += alphas[i]*scs[i].Apply(im);
	
	return value;
}

int AdaBoostClassifier::ApplyImagePatch(const IntImage& im) const
{
	return (GetValue(im)>=thresh)?1:0;
}

void SingleFeatureClassifier(const int* const labels,SimpleClassifier& sc,const int* const indexes);
void AppendAdaBoostClassifier(const AdaBoostClassifier& ada);

#endif
