
#ifndef __POS_H__
#define __POS_H__

#ifdef POS_Dll_EXPORTS
#define POS_Dll_API __declspec(dllexport)
#else
#define POS_Dll_API __declspec(dllimport)
#endif

//====================================================
// POS_Dll exported functions
//====================================================

extern "C"
{
/*
* Function:		
*				get name of first protocol.
* Return value:
*				protocol name. return NULL if no protocol is supported.
*				NOTE: don't modify it.
*
* Example:
*
*	char *protocol = POS_GetFirstProtocol();
*
*	while( protocol != NULL )
*	{
*		// do something with protocol, but don't modify it.
*
*		protocol = POS_GetNextProtocol( protocol );
*	}
*/
POS_Dll_API char *POS_GetFirstProtocol();

/*
* Function:		
*				get name of next protocol.
* Parameters:
*	pCurrProtocol:	protocol name
* Return value:
*				protocol name. 
*				return NULL if the end of protocol list is reached.
*/
POS_Dll_API char *POS_GetNextProtocol( char *pCurrProtocol );

/*
* Function:		
*				open port, read data,and start processing it.
* Parameters:
*	pProtocolName:	protocol name
*	nPort:			port number
*	ByteSize:		byte Size
*	Stop Bits:		stop bits
*	Parity:			parity type
*	BaudRate:		baud rate	
* Return value:
*				handle to pseudo COM. 
*				return INVALID_HANDLE_VALUE if the end of protocol list 
*				is reached. 
*				NOTE: it is not a real handle to COM. Don't use ReadFile()
*				or other similar functions to access it.
*
* Example:
*	HANDLE hCom = POS_Open( "EagleVision0S",	
*							1,	
*							7,
*							ONESTOPBIT,
*							NOPARITY,
*							1200 );
*		
*	if( hCom == INVALID_HANDLE_VALUE )
*		return ;
*		
*	char buffer[256];
*	while( !exitFlag ) 
*	{
*		UINT count = POS_Read( hCom, buffer, sizeof(buffer) ); 
*
*		if( count > 0 )
*		{
*			//read data from buffer, and handle it
*		}
*	}
*		
*	POS_Close( hCom ); 		
*/
POS_Dll_API HANDLE POS_Open( char *pProtocolName, 
							 int nPort, 
							 BYTE ByteSize, 
							 BYTE StopBits, 
							 BYTE Parity, 
							 DWORD BaudRate ); 

/*
* Function:		
*				stop processing, and close port.
* Parameters:
*	hCom:		handle to pseudo COM
*/
POS_Dll_API void POS_Close( HANDLE hCom );

/*
* Function:		
*				read binary data.
* Parameters:
*	hCom:			handle to pseudo COM
*	lpBuf:			buffer to store data into.
*	nCount:			byte length of buffer.
* Return value:
*				byte count of data stored actually. 
*/
POS_Dll_API UINT POS_Read( HANDLE hCom, void* lpBuf, UINT nCount );

}

#endif
