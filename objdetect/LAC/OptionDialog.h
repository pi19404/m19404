#pragma once
#include "afxwin.h"


// OptionDialog dialog

class OptionDialog : public CDialog
{
	DECLARE_DYNAMIC(OptionDialog)

public:
	OptionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptionDialog();

// Dialog Data
	enum { IDD = IDD_Option };

protected:
	void UpdateStatus();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_ratio;
	int m_size;
	int m_fs;
	int m_lc;
	afx_msg void OnBnClickedAdaboost();
	afx_msg void OnBnClickedAsymboost();
	afx_msg void OnBnClickedFfs();
	afx_msg void OnBnClickedOriginal();
	afx_msg void OnBnClickedLac();
	afx_msg void OnBnClickedFda();
};
