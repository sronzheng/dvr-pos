// Target.h : main header file for the TARGET application
//

#if !defined(AFX_TARGET_H__E33C57E8_066D_4131_A972_7EE3C1E3E981__INCLUDED_)
#define AFX_TARGET_H__E33C57E8_066D_4131_A972_7EE3C1E3E981__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTargetApp:
// See Target.cpp for the implementation of this class
//

class CTargetApp : public CWinApp
{
public:
	CTargetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTargetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGET_H__E33C57E8_066D_4131_A972_7EE3C1E3E981__INCLUDED_)
