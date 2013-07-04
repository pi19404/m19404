// FaceDetect.h : main header file for the FACEDETECT application
//

#if !defined(AFX_FACEDETECT_H__A753294B_37D7_423D_AE3D_0F41AA7185BB__INCLUDED_)
#define AFX_FACEDETECT_H__A753294B_37D7_423D_AE3D_0F41AA7185BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFaceDetectApp:
// See FaceDetect.cpp for the implementation of this class
//

class CFaceDetectApp : public CWinApp
{
public:
	CFaceDetectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceDetectApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFaceDetectApp)
 void OnAppAbout();
 void OnFileOpen();
	//}}AFX_MSG
    //DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACEDETECT_H__A753294B_37D7_423D_AE3D_0F41AA7185BB__INCLUDED_)
