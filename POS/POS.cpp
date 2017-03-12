// POS.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#pragma warning(disable:4786)
#include <map>

#include "pos.h"
#include "serialport.h"
#include "ParserFactory.h"

extern CParserFactory g_parserFactory;

HANDLE g_mtxMapAccess = NULL;
std::map<int, CSerialPort *> g_serialPortMap;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_mtxMapAccess = CreateMutex( NULL,		// no security attributes
										  FALSE,	// initial without ownership
										  NULL );	// unnamed 
			if( g_mtxMapAccess == NULL )
				return FALSE;

			break;

		case DLL_PROCESS_DETACH:
			CloseHandle( g_mtxMapAccess );
			break;

		default:
			break;
    }

    return TRUE;
}

extern "C" POS_Dll_API char *POS_GetFirstProtocol()
{
	return g_parserFactory.getFirstProtocol(); 
}

extern "C" POS_Dll_API char *POS_GetNextProtocol( char *pCurrProtocol )
{
	return g_parserFactory.getNextProtocol( pCurrProtocol );
}

extern "C" POS_Dll_API HANDLE POS_Open( char *pProtocolName, 
									    int nPort, 
										BYTE ByteSize, 
										BYTE StopBits, 
										BYTE Parity, 
										DWORD BaudRate )
{
	CSerialPort *serialPort = new CSerialPort;
	if( serialPort == NULL )
		return NULL;

	if ( ! serialPort->open( pProtocolName, 
							 nPort,  
							 ByteSize,
							 StopBits,
							 Parity,
							 BaudRate ) )
	{
		delete serialPort;

		return NULL;
	}

	if( WaitForSingleObject( g_mtxMapAccess, INFINITE ) == WAIT_OBJECT_0 )
	{
		typedef std::pair< int, CSerialPort * > Port_Pair;

		g_serialPortMap.insert( Port_Pair( nPort, serialPort ) );

		ReleaseMutex( g_mtxMapAccess );
	}
	else
	{
		serialPort->close();
		delete serialPort;

		return NULL;
	}
	
	return (HANDLE) nPort;
}

extern "C" POS_Dll_API void POS_Close( HANDLE hCom )
{
	// find the CSerialPort object belonged to specified serial port, 
	// and pick it up
	int port = (int) hCom;
	CSerialPort *serialPort = NULL;

	if( WaitForSingleObject( g_mtxMapAccess, INFINITE ) == WAIT_OBJECT_0 )
	{
		std::map< int, CSerialPort * >::iterator iter = 
			g_serialPortMap.find( port );

		if( iter == g_serialPortMap.end() )
		{
			ReleaseMutex( g_mtxMapAccess );
			return ;
		}
		
		serialPort = iter->second;
		
		// close it
		g_serialPortMap.erase( iter );
		
		ReleaseMutex( g_mtxMapAccess );
	}
	else 
		return;

	serialPort->close();

	delete serialPort;
}

extern "C" POS_Dll_API UINT POS_Read( HANDLE hCom, void* lpBuf, UINT nCount )
{
	// find the CSerialPort object belonged to specified serial port, 
	// and pick it up
	int port = (int) hCom;
	CSerialPort *serialPort = NULL;

	if( WaitForSingleObject( g_mtxMapAccess, INFINITE ) == WAIT_OBJECT_0 )
	{
		std::map< int, CSerialPort * >::iterator iter = 
			g_serialPortMap.find( port );

		if( iter == g_serialPortMap.end() )
		{
			ReleaseMutex( g_mtxMapAccess );
			return 0;
		}
		
		serialPort = iter->second;

		ReleaseMutex( g_mtxMapAccess );
	}
	else
		return 0;

	//read data from it
	return serialPort->read( lpBuf, nCount);
}
