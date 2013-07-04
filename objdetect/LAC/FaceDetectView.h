// FaceDetectView.h : interface of the CFaceDetectView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEDETECTVIEW_H__43C12073_634A_463C_BDCD_985F6621A9DB__INCLUDED_)
#define AFX_FACEDETECTVIEW_H__43C12073_634A_463C_BDCD_985F6621A9DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFaceDetectView : public CView
{
protected: // create from serialization only
	CFaceDetectView();
	DECLARE_DYNCREATE(CFaceDetectView)

// Attributes
public:
	CFaceDetectDoc* GetDocument();

	int index;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceDetectView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFaceDetectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFaceDetectView)
	afx_msg void OnViewTrainimage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in FaceDetectView.cpp
inline CFaceDetectDoc* CFaceDetectView::GetDocument()
   { return (CFaceDetectDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACEDETECTVIEW_H__43C12073_634A_463C_BDCD_985F6621A9DB__INCLUDED_)
