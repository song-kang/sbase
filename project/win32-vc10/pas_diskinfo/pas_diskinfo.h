// pas_diskinfo.h : main header file for the PAS_DISKINFO application
//

#if !defined(AFX_PAS_DISKINFO_H__5B2133FC_A158_4A36_A305_885356FD2903__INCLUDED_)
#define AFX_PAS_DISKINFO_H__5B2133FC_A158_4A36_A305_885356FD2903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPas_diskinfoApp:
// See pas_diskinfo.cpp for the implementation of this class
//

class CPas_diskinfoApp : public CWinApp
{
public:
	CPas_diskinfoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPas_diskinfoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPas_diskinfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAS_DISKINFO_H__5B2133FC_A158_4A36_A305_885356FD2903__INCLUDED_)
