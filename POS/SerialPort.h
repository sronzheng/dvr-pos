// SerialPort1.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT1_H__650FEE97_B7FE_4AEA_954B_6D5C4F9E1C5F__INCLUDED_)
#define AFX_SERIALPORT1_H__650FEE97_B7FE_4AEA_954B_6D5C4F9E1C5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"
#include "receiver.h"
#include "binarystream.h"

/*
* Class of processing serial port data.		
*/
class CSerialPort  
{
public:

	/*
	* Function:		
	*				open port, read data from it, process it, and save
	*				the result as binary data in a stream.
	* Parameters:
	*	protocolName:	POS protocol name.
	*	port:			port number
	*	byteSize:		byte size
	*	stopBits:		stop bits
	*   parity:			parity type
	*	baudRate:		baud rate
	* Return value:
	*				indicate whether the port is open successfully. 
	*/
	BOOL open( char *protocolName, 
			   int  port,
			   BYTE byteSize,
			   BYTE stopBits,
			   BYTE parity,
			   DWORD baudRate );

	/*
	* Function:		
	*				stop processing, and close port.
	*/
	void close();

	/*
	* Function:		
	*				read data from binary data stream.
	* Parameters:
	*	buf:		buffer to store data into.
	*	count:		byte length of buffer
	* Return value:
	*				byte count stored actually. 
	*/
	int read( void *buf, int count );

	CSerialPort();

	~CSerialPort();

private:

	static VOID ThreadProc (PVOID pVoid);

	HANDLE m_hCom;

	HANDLE m_mtxAccessStream;
	CBinaryStream *m_stream;
	CReceiver *m_receiver;
	CParser *m_parser;

	HANDLE m_evnThreadExitLoop;
	HANDLE m_evnThreadEnded;
};

#endif // !defined(AFX_SERIALPORT1_H__650FEE97_B7FE_4AEA_954B_6D5C4F9E1C5F__INCLUDED_)
