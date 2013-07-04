// FaceDetectDoc.h : interface of the CFaceDetectDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEDETECTDOC_H__34E87AA0_3D69_4478_9A8F_09A4DD504E0A__INCLUDED_)
#define AFX_FACEDETECTDOC_H__34E87AA0_3D69_4478_9A8F_09A4DD504E0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFaceDetectDoc : public CDocument
{
protected: // create from serialization only
	CFaceDetectDoc();
	DECLARE_DYNCREATE(CFaceDetectDoc)

// Attributes
public:
	IntImage image;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceDetectDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFaceDetectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFaceDetectDoc)
	afx_msg void OnTestAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTrain();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACEDETECTDOC_H__34E87AA0_3D69_4478_9A8F_09A4DD504E0A__INCLUDED_)
