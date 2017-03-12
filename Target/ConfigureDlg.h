#if !defined(AFX_CONFIGUREDLG_H__9C5FADD1_7D2E_4B9B_A51D_D2979780536F__INCLUDED_)
#define AFX_CONFIGUREDLG_H__9C5FADD1_7D2E_4B9B_A51D_D2979780536F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog

class CConfigureDlg : public CDialog
{
// Construction
public:
	CConfigureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigureDlg)
	enum { IDD = IDD_CONFIGUREDLG_DIALOG };
	CComboBox	m_protocolCtl;
	CString	m_cameraView;
	CString	m_flowBits;
	CString	m_parity;
	CString	m_port;
	CString	m_speed;
	CString	m_stopBits;
	CString	m_xmlFile;
	//}}AFX_DATA

	CString m_protocol;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREDLG_H__9C5FADD1_7D2E_4B9B_A51D_D2979780536F__INCLUDED_)
