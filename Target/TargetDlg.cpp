// TargetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Target.h"

#pragma warning(disable:4786)
#include "TargetDlg.h"
#include "ConfigureDlg.h"

#include "wndreceiver.h"
#include "xmlfilereceiver.h"

#include "pos.h"
#include "posstruct.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================
//	CommThread
//===========================================================
UINT CTargetDlg::CommThread(LPVOID pParam)
{
	PORT_THREAD_STRUCT *stPort = (PORT_THREAD_STRUCT *)pParam;

	HANDLE hCom = stPort->hCom;
	std::list<CReceiver *> &receivers = stPort->receivers;
	BOOL &exitFlag = stPort->exitFlag;

	OutputDebugString( " \nTarget: Start reading PSEDUO port thread!\n " );

	char buffer[256];

	while( !exitFlag ) 
	{ 
		UINT count = POS_Read( hCom, buffer, sizeof(buffer) ); 

		if( count <= 0 )
		{
			Sleep(100);
		}
		else
		{

#ifdef _DEBUG
			char msg[512];
			sprintf(msg, "\nTarget: Read data: %s\n", buffer);

			OutputDebugString( msg );
#endif

			POS_DATA *data = (POS_DATA *)buffer;

			if( strcmp( data->keyword, "Transaction" ) == 0 )
			{
				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setTransaction( ); 
				}
			}
			else if( strcmp( data->keyword, "DateTime" ) == 0 )
			{
				UINT64 nano100second  = data->dateTime * 10000;
				
				FILETIME fileTime;
				
				fileTime.dwHighDateTime = nano100second >> 32;
				fileTime.dwLowDateTime = nano100second & 0xFFFFFFFF;
				
				SYSTEMTIME systemTime;
				FileTimeToSystemTime( &fileTime, &systemTime);
				
				std::list<CReceiver *>::iterator iter;
				
				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
						 (*iter)->setTime( systemTime.wYear,
										   systemTime.wMonth,
										   systemTime.wDay,
										   systemTime.wHour,
										   systemTime.wMinute ); 
				}

			}
			else if( strcmp( data->keyword, "Item" ) == 0 )
			{
				MONEY_AMOUNT unitPrice;
				unitPrice.dollar	= data->itemData.unitPrice / 100;
				unitPrice.cent		= data->itemData.unitPrice % 100;
				
				int quantity = data->itemData.quantity;

				MONEY_AMOUNT price;
				price.dollar	= data->itemData.price / 100;
				price.cent		= data->itemData.price % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setItem( data->itemData.name,
									  unitPrice,
									  quantity,
									  price);
				}
			}
			else if( strcmp( data->keyword, "Item Voided" ) == 0 )
			{
				MONEY_AMOUNT amount;
				memset( &amount, 0, sizeof(amount) );

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setItemVoided( amount ); 
				}

			}
			else if( strcmp( data->keyword, "Subtotal" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setSubtotal( amount ); 
				}

			}
			else if( strcmp( data->keyword, "Tax" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setTax( amount ); 
				}

			}
			else if( strcmp( data->keyword, "Total" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setTotal( amount ); 
				}

			}
			else if( strcmp( data->keyword, "Cash" ) == 0)
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setPaymentType( CASH, amount ); 
				}
			}
			else if( strcmp( data->keyword, "Check" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setPaymentType( CHECK, amount ); 
				}

			}
			else if( strcmp( data->keyword, "Credit" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setPaymentType( CREDIT, amount ); 
				}

			}
			else if( strcmp( data->keyword, "ATM" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setPaymentType( ATM, amount ); 
				}

			}else if( strcmp( data->keyword, "Gift certificate" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setPaymentType( GIFT_CERTIFICATE, amount ); 
				}

			}else if( strcmp( data->keyword, "Change" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setChange( amount ); 
				}

			}else if( strcmp( data->keyword, "Refund" ) == 0 )
			{
				MONEY_AMOUNT amount;
				amount.dollar	= data->amount / 100;
				amount.cent		= data->amount % 100;

				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setRefund( amount ); 
				}

			}
			else if( strcmp( data->keyword, "Transaction Voided" ) == 0 )
			{
				std::list<CReceiver *>::iterator iter;

				for( iter = receivers.begin();
					 iter != receivers.end(); 
					 iter++ )
				{
					(*iter)->setTransactionVoided( ); 
				}
			}
		}
	} 

	POS_Close( hCom ); 

	OutputDebugString( " \nTarget: Exit reading PSEDUO port thread!\n " );

	return 0;
}

void CTargetDlg::OnConfigure() 
{
	CConfigureDlg dlg;
	int IDC = dlg.DoModal();

	int port;
	if( dlg.m_port.Compare( "COM1" ) == 0 )
		port = 1;
	else if( dlg.m_port.Compare( "COM2" ) == 0 )
		port = 2;
	else if( dlg.m_port.Compare( "COM3" ) == 0 )
		port = 3;
	else if( dlg.m_port.Compare( "COM4" ) == 0 )
		port = 4;

	if( IDC == IDOK)
	{
		// get parameters.
		char protocol[32];
		strcpy( protocol, dlg.m_protocol.GetBuffer(32) );  
		
		int baudRate = atoi( dlg.m_speed.GetBuffer(16) );
		
		int parity;
		if( dlg.m_parity.Compare( "Even" ) == 0 )
			parity = EVENPARITY;
		else if( dlg.m_parity.Compare( "Odd" ) == 0 )
			parity = ODDPARITY;
		else if( dlg.m_parity.Compare( "None" ) == 0 )
			parity = NOPARITY;

		int stopBits;
		if( dlg.m_stopBits.Compare( "1" ) == 0  )
			stopBits = ONESTOPBIT;
		else if( dlg.m_stopBits.Compare( "1.5" ) == 0  )
			stopBits = ONE5STOPBITS;
		else if( dlg.m_stopBits.Compare( "2" ) == 0  )
			stopBits = TWOSTOPBITS;

		int flowBits = atoi( dlg.m_flowBits.GetBuffer(16) );

		CListBox *view;
		CStatic *tip;
		if( dlg.m_cameraView.Compare( "VIEW 1" ) == 0 )
		{
			view = &m_view1;
			tip = &m_tip1;
		}
		else if( dlg.m_cameraView.Compare( "VIEW 2" ) == 0 )
		{
			view = &m_view2;
			tip = &m_tip2;
		}

		std::list<PORT_THREAD_STRUCT *>::iterator iter;
		
		for( iter = m_stPorts.begin(); iter != m_stPorts.end(); iter++ )
		{
			PORT_THREAD_STRUCT *p = *iter;
			if( p->portNo == port )
			{
				AfxMessageBox("Failed. COM port is open !");
				return ;
			}
			else if( p->tipWnd == tip )
			{
				AfxMessageBox("Failed. View in use !");
				return ;
			}
		}

		char buf[32];
		sprintf( buf, "COM %d", port);
		tip->SetWindowText( buf );

		CReceiver *wndReceiver = new CWndReceiver( view );

		std::ofstream *of = new std::ofstream();

		of->open( dlg.m_xmlFile.GetBuffer(128) ); 
		if( ! of->is_open() )
		{
			AfxMessageBox("Failed. Cannot open XML file !");

			delete of;
			delete wndReceiver;

			return ;
		}

		CReceiver *xmlReceiver = new CXmlFileReceiver( *of );
		
		// open pseudo port, and start to read data. 
		HANDLE hCom = POS_Open( protocol, 
								port,	
								flowBits,
								stopBits,
								parity,
								baudRate );
		
		if( hCom == NULL )
		{
			AfxMessageBox("Failed. Cannot open COM port !");

			return ;
		}
		
		PORT_THREAD_STRUCT *stPort = new PORT_THREAD_STRUCT();
		stPort->hCom		= hCom;

		stPort->portNo		= port;
		stPort->tipWnd		= tip;

		stPort->receivers.push_back( wndReceiver );
		stPort->receivers.push_back( xmlReceiver );
		stPort->xmlOf		= of;

		stPort->exitFlag	= FALSE;
		
		m_stPorts.push_back( stPort );
		
		AfxBeginThread(CommThread, stPort);
	}
	else if( IDC == IDC_STOP )
	{
		// inform work thread to exit, and clean.
		std::list<PORT_THREAD_STRUCT *>::iterator iter;
		
		for( iter = m_stPorts.begin(); iter != m_stPorts.end(); iter++ )
		{
			PORT_THREAD_STRUCT *p = *iter;

			if( p->portNo == port )
			{
				m_stPorts.erase( iter ); 

				p->exitFlag = TRUE;

				p->tipWnd->SetWindowText( "NOT USED" );  

				std::list<CReceiver *> &receivers = p->receivers;

				std::list<CReceiver *>::iterator itReceiver;

				for( itReceiver = receivers.begin();
					 itReceiver != receivers.end(); 
					 itReceiver++ )
				{
					delete (*itReceiver); 
				}

				delete p->xmlOf; 

				delete p;

				return ;
			}
		}

		AfxMessageBox("Failed. COM port is NOT open !");
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetDlg dialog

CTargetDlg::CTargetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTargetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTargetDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTargetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetDlg)
	DDX_Control(pDX, IDC_TIP2, m_tip2);
	DDX_Control(pDX, IDC_TIP1, m_tip1);
	DDX_Control(pDX, IDC_VIEW2, m_view2);
	DDX_Control(pDX, IDC_VIEW1, m_view1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTargetDlg, CDialog)
	//{{AFX_MSG_MAP(CTargetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetDlg message handlers

BOOL CTargetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTargetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTargetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTargetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

