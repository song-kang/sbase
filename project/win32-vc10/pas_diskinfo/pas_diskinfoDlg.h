// pas_diskinfoDlg.h : header file
//

#if !defined(AFX_PAS_DISKINFODLG_H__86365603_21C0_4DF6_8A8E_8C6B88FC4838__INCLUDED_)
#define AFX_PAS_DISKINFODLG_H__86365603_21C0_4DF6_8A8E_8C6B88FC4838__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPas_diskinfoDlg dialog
#include "DiskSN.h"
class CPas_diskinfoDlg : public CDialog
{
// Construction
public:
	CPas_diskinfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPas_diskinfoDlg)
	enum { IDD = IDD_PAS_DISKINFO_DIALOG };
	CString	m_sInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPas_diskinfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPas_diskinfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAS_DISKINFODLG_H__86365603_21C0_4DF6_8A8E_8C6B88FC4838__INCLUDED_)
