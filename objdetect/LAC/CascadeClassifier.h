struct CascadeClassifier
{
	int count;
	AdaBoostClassifier* ac;

	CascadeClassifier();
	~CascadeClassifier();
	void Clear(void);
	CascadeClassifier& operator=(const CascadeClassifier& source);

	void WriteToFile(ofstream& f) const;
	void ReadFromFile(ifstream& f);
	void LoadDefaultCascade(void);

	virtual const int ApplyImagePatch(const IntImage& im) const;
    virtual IplImage * ApplyOriginalSize(IntImage& original,const CStdString filename) ;
    virtual void ApplyOriginalSizeForInputBoosting(const CStdString filename,int& pointer) const;

    IplImage * DrawResults(IntImage& image, const vector<QRect>& results) const;

	virtual const bool OneRound(const int round);
};

void PostProcess(vector<QRect>& result, const int combine_min);
