// SerialPort1.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

#include "streamreceiver.h"
#include "ParserFactory.h"

#include <process.h>

CParserFactory g_parserFactory;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()
: m_hCom( NULL ),
  m_parser( NULL ),
  m_receiver( NULL ),
  m_stream( NULL ),
  m_mtxAccessStream( NULL ),
  m_evnThreadExitLoop( NULL ),
  m_evnThreadEnded( NULL )
{
}

CSerialPort::~CSerialPort()
{

}

BOOL CSerialPort::open( char *protocolName, 
					    int  port,
						BYTE byteSize,
						BYTE stopBits,
						BYTE parity,
						DWORD baudRate )
{
	// prepare binary stream object, receiver, and parser.
	m_mtxAccessStream = CreateMutex( NULL,	// no security attributes
									 FALSE, // initial without ownership
									 NULL);	// unnamed 
	if( m_mtxAccessStream == NULL )
		goto clean_and_exit;

	m_stream = new CBinaryStream( m_mtxAccessStream );
	if( m_stream == NULL )
		goto clean_and_exit;

	m_receiver = new CStreamReceiver( *m_stream );
	if( m_receiver == NULL )
		goto clean_and_exit;

	m_parser = g_parserFactory.createParser( protocolName, 
											 m_receiver );
	if( m_parser == NULL )
		goto clean_and_exit;

	// prepare event object,used to inform parsing thread to exit. 
	m_evnThreadExitLoop = CreateEvent( NULL,	// no security attributes
									   TRUE,	// manual-reset
									   FALSE,	// initial nonsignaled.
									   NULL);	// unnamed
	if( m_evnThreadExitLoop == NULL ) 
		goto clean_and_exit;

	// prepare event object,used to inform that parsing thread is already ended. 
	m_evnThreadEnded = CreateEvent( NULL,	// no security attributes
									TRUE,	// manual-reset		
									FALSE,	// intial nonsignaled.
									NULL);	// unnamed
	if( m_evnThreadEnded == NULL ) 
		goto clean_and_exit;

	char comName[128];
    sprintf( comName, "\\\\.\\\\COM%d", port );

	// open port, and set its parameters.
	m_hCom = CreateFile( comName,		// communication port string (COMX)
						 GENERIC_READ,	// read only
						 0,				// comm devices must be opened with exclusive access
						 NULL,			// no security attributes
						 OPEN_EXISTING,	// comm devices must use OPEN_EXISTING
						 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,// Async I/O
						 0);			// template must be 0 for comm devices

	if( m_hCom == NULL )
		goto clean_and_exit;

	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 1000;
	CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;

	if( !SetCommTimeouts( m_hCom, &CommTimeouts ) )
		goto clean_and_exit;

	DCB dcb;
	memset( &dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof( dcb );

	if( !GetCommState( m_hCom, &dcb) )
		goto clean_and_exit;

	dcb.fParity = TRUE;
	dcb.BaudRate = baudRate;
	dcb.Parity = parity;
	dcb.StopBits = stopBits;
	dcb.ByteSize = byteSize;

	if( !SetCommState( m_hCom, &dcb) )
		goto clean_and_exit;

	// start to read and parse data.
	_beginthread(ThreadProc, 0, this) ;

	return TRUE;

	//if any problem occurs, clean all.
clean_and_exit:

	if( m_hCom != NULL )
	{
		CloseHandle( m_hCom );
		m_hCom = NULL;
	}

	if( m_evnThreadEnded != NULL )
	{
		CloseHandle( m_evnThreadEnded );
		m_evnThreadEnded = NULL;
	}

	if( m_evnThreadExitLoop != NULL )
	{
		CloseHandle( m_evnThreadExitLoop );
		m_evnThreadExitLoop = NULL;
	}

	delete m_parser;
	m_parser = NULL;

	delete m_receiver;
	m_receiver = NULL;

	delete m_stream;
	m_stream = NULL;

	if( m_mtxAccessStream != NULL )
	{
		CloseHandle( m_mtxAccessStream );
		m_mtxAccessStream = NULL;
	}

	return FALSE;
}

void CSerialPort::close()
{
	// inform and wait reading-paring thread to exit
	SetEvent( m_evnThreadExitLoop );

	WaitForSingleObject( m_evnThreadEnded, INFINITE );

	//clean all
	CloseHandle( m_hCom );
	m_hCom = NULL;

	CloseHandle( m_evnThreadEnded );
	m_evnThreadEnded = NULL;

	CloseHandle( m_evnThreadExitLoop );
	m_evnThreadExitLoop = NULL;

	delete m_parser;
	m_parser = NULL;

	delete m_receiver;
	m_receiver = NULL;

	delete m_stream;
	m_stream = NULL;

	CloseHandle( m_mtxAccessStream );
	m_mtxAccessStream = NULL;
}

int CSerialPort::read( void *buf, int count )
{
	return m_stream->read( buf, count);
}

VOID CSerialPort::ThreadProc (PVOID pVoid)
{
	CSerialPort *serialPort = (CSerialPort*) pVoid;

	HANDLE hCom	= serialPort->m_hCom; 
	CParser *parser = serialPort->m_parser; 
	HANDLE evnExitLoop = serialPort->m_evnThreadExitLoop; 
	HANDLE evnEnded = serialPort->m_evnThreadEnded; 

	OutputDebugString( "\nPOS: Start reading port thread!\n" );

	//clean input buffer
	PurgeComm( hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT );

	OVERLAPPED overLapped = { 0 };
	overLapped.hEvent = CreateEvent( NULL,	// no security attributes		
									 TRUE,	// manual-reset
									 FALSE,	// intial nonsignaled.
									 NULL); // unnamed 

	if( overLapped.hEvent == NULL )
		return ;

	HANDLE events[2];
	events[0] = evnExitLoop;
	events[1] = overLapped.hEvent; 
 
	char buf[512];
	buf[0] = 0;

	char *bufRead = buf;

	while( TRUE )
	{ 
		DWORD bytesRead = 0;

		// leave one byte in buffer, to place '\0' in.
		ReadFile( hCom, bufRead, sizeof(buf) - (bufRead-buf) - 1, &bytesRead, &overLapped );

		DWORD ret = WaitForMultipleObjects( 2,			// length of event array
											events,		// event array
											FALSE,		// return if any event is in signaled
											INFINITE );	// 

		if(  ret != WAIT_OBJECT_0 && ret != ( WAIT_OBJECT_0 + 1 ) )
			continue;

		// if exit flag signaled, exit loop and end.
		if( ( ret -  WAIT_OBJECT_0 ) == 0 )
			break;

		if( ! GetOverlappedResult( hCom, &overLapped, &bytesRead, TRUE ) )
			continue;

		OutputDebugString( "\nPOS: Return from reading port\n" );

		bufRead[bytesRead] = '\0';

		bufRead = parser->parse( buf );
	} 

	CloseHandle( overLapped.hEvent );

	OutputDebugString( "\nPOS: Exit reading port thread!\n" );

	SetEvent( evnEnded );

	_endthread ();

	return ;
}
