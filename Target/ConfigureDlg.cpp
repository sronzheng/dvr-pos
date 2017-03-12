// ConfigureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Target.h"
#include "ConfigureDlg.h"

#include "Pos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog


CConfigureDlg::CConfigureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureDlg)
	m_cameraView = _T("");
	m_flowBits = _T("");
	m_parity = _T("");
	m_port = _T("");
	m_speed = _T("");
	m_stopBits = _T("");
	m_xmlFile = _T("");
	//}}AFX_DATA_INIT
}


void CConfigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureDlg)
	DDX_Control(pDX, IDC_PROTOCOL, m_protocolCtl);
	DDX_CBString(pDX, IDC_CAMERAVIEW, m_cameraView);
	DDX_CBString(pDX, IDC_FLOWBITS, m_flowBits);
	DDX_CBString(pDX, IDC_PARITY, m_parity);
	DDX_CBString(pDX, IDC_PORT, m_port);
	DDX_CBString(pDX, IDC_SPEED, m_speed);
	DDX_CBString(pDX, IDC_STOPBITS, m_stopBits);
	DDX_CBString(pDX, IDC_XMLFILE, m_xmlFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigureDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigureDlg)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg message handlers

BOOL CConfigureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char *protocol = POS_GetFirstProtocol();

	while( protocol != NULL )
	{
		m_protocolCtl.AddString( protocol ); 

		protocol = POS_GetNextProtocol( protocol );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigureDlg::OnOK() 
{
	// start button pressed.
	UpdateData( TRUE );

	if( m_port.IsEmpty() )
	{
		AfxMessageBox("Please select a COM port !");
		return ;
	}

	int sel = m_protocolCtl.GetCurSel(); 
	if( sel == CB_ERR )
	{
		AfxMessageBox("Please select a Protocol !");
		return ;
	}

	m_protocolCtl.GetLBText( sel, m_protocol);

	if( m_speed.IsEmpty())
	{
		AfxMessageBox("Please select speed !");
		return ;
	}

	if( m_parity.IsEmpty() )
	{
		AfxMessageBox("Please select parity !");
		return;
	}

	if( m_stopBits.IsEmpty())
	{
		AfxMessageBox("Please select stop bits !");
		return ;
	}

	if( m_flowBits.IsEmpty())
	{
		AfxMessageBox("Please select flow bits !");

		return ;
	}

	if( m_cameraView.IsEmpty() )
	{
		AfxMessageBox("Please select a camera view !");
		return ;
	}

	if( m_xmlFile.IsEmpty() )
	{
		AfxMessageBox("Please specify a xml file name !");
		return ;
	}

	CDialog::OnOK();
}

void CConfigureDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CConfigureDlg::OnStop() 
{
	// stop button pressed.
	UpdateData( TRUE );

	if( m_port.IsEmpty() )
	{
		AfxMessageBox("Please select a COM port !");
		return ;
	}
	
	EndDialog( IDC_STOP );
}
