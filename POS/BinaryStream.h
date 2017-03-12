// BinaryStream.h: interface for the CBinaryStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYSTREAM_H__DCBBFA76_419D_4D46_92CF_4BB4746DED42__INCLUDED_)
#define AFX_BINARYSTREAM_H__DCBBFA76_419D_4D46_92CF_4BB4746DED42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

/*
* binary data stream class.
* its WRITE and READ access operation is serialized		
*/
class CBinaryStream  
{
public:
	
	/*
	* Function:		
	*				write data into stream.
	*				multithread access is serialized.
	* Parameters:
	*	buf:		data to be wroten
	*   count:		byte count to be wroten
	* Return value:
	*				byte count wroten actually.
	*/
	int write( void *buf, int count );

	/*
	* Function:		
	*				read data from stream.
	*				multithread access is serialized.
	* Parameters:
	*	buf:		buffer to store data in.
	*   count:		buffer length
	* Return value:
	*				byte count stored actually.
	*/
	int read( void *buf, int count );

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	mtxAccess:		used to serialize read and write operation.
	*/
	CBinaryStream( HANDLE mtxAccess );

	~CBinaryStream();

private:

	std::list< char * > m_buffers;

	HANDLE m_mtxAccess;
};

#endif // !defined(AFX_BINARYSTREAM_H__DCBBFA76_419D_4D46_92CF_4BB4746DED42__INCLUDED_)
