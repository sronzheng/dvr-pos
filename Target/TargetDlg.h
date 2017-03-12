// TargetDlg.h : header file
//

#if !defined(AFX_TARGETDLG_H__6D5DCF12_6D4F_4271_9C02_CED5EEFB24E8__INCLUDED_)
#define AFX_TARGETDLG_H__6D5DCF12_6D4F_4271_9C02_CED5EEFB24E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include <list>

#include "receiver.h"

typedef struct _PORT_THREAD_STRUCT
{
	HANDLE					hCom;			// handle to pseduo port

	int						portNo;			// actual port number
	CStatic	  *				tipWnd;			
	std::list<CReceiver *>	receivers;	
	std::ofstream *			xmlOf;

	BOOL					exitFlag;		// exit flag, inform thread to exit. 	

} PORT_THREAD_STRUCT, *PPORT_THREAD_STRUCT;

/////////////////////////////////////////////////////////////////////////////
// CTargetDlg dialog

class CTargetDlg : public CDialog
{
// Construction
public:
	CTargetDlg(CWnd* pParent = NULL);	// standard constructor

	static UINT CommThread(LPVOID pParam);

	std::list<PORT_THREAD_STRUCT *> m_stPorts;

// Dialog Data
	//{{AFX_DATA(CTargetDlg)
	enum { IDD = IDD_TARGET_DIALOG };
	CStatic	m_tip2;
	CStatic	m_tip1;
	CListBox	m_view2;
	CListBox	m_view1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTargetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConfigure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGETDLG_H__6D5DCF12_6D4F_4271_9C02_CED5EEFB24E8__INCLUDED_)
