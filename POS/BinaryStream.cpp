// BinaryStream.cpp: implementation of the CBinaryStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BinaryStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBinaryStream::CBinaryStream( HANDLE mtxAccess )
 : m_mtxAccess( mtxAccess )
{

}

CBinaryStream::~CBinaryStream()
{
	std::list<char *>::iterator iter;

	for( iter = m_buffers.begin(); iter != m_buffers.end(); iter++)
	{
		char *p = *iter;
		
		*iter = NULL;

		delete []p;
	}
}

int CBinaryStream::write( void *buf, int count )
{
	char *p = new char[count + 4];
	if( p == NULL )
		return 0;

	// save the length(count) at the beginning of buffer, 
	// followed by binary data. 
	*(int *)p = count;

	memcpy( p + 4, buf, count);

	if( WaitForSingleObject( m_mtxAccess, INFINITE ) == WAIT_OBJECT_0 )
	{
		m_buffers.push_back( p );

		OutputDebugString( "\nPOS: binary data WROTTEN !\n" );

		ReleaseMutex( m_mtxAccess );
	}
	else
	{
		delete []p;

		return 0;
	}

	return count;
}

int CBinaryStream::read( void *buf, int count )
{
	char *p = NULL;

	if( WaitForSingleObject( m_mtxAccess, INFINITE ) == WAIT_OBJECT_0 )
	{
		std::list<char *>::iterator iter = m_buffers.begin();
		if( iter == m_buffers.end() )
		{
			ReleaseMutex( m_mtxAccess );

			return 0;
		}
		
		p = *iter;

		OutputDebugString( "\nPOS: binary data RED !\n" );
		
		m_buffers.pop_front();

		ReleaseMutex( m_mtxAccess );
	}
	else
		return 0;

	// read the length(len) from the beginning of buffer, 
	// and then read binary data. 
	int len = *(int *)p;

	memcpy( buf, p + 4, len); 

	delete []p;

	return len;
}

