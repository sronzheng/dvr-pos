// BinaryStreamTest.cpp: implementation of the CBinaryStreamTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BinaryStreamTest.h"

#include "binarystream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CBinaryStreamTest );

CBinaryStreamTest::CBinaryStreamTest()
{

}

CBinaryStreamTest::~CBinaryStreamTest()
{

}

void CBinaryStreamTest::testReadWrite1()
{

	HANDLE mutex = CreateMutex( NULL, FALSE, NULL ); 
	CPPUNIT_ASSERT( mutex != NULL );

	// Write 1 item, try to read 1 item.
	CBinaryStream stream( mutex ) ;

	char keyword[] = "Item";
	char itemName[] = "TEST SKU";
	int unitPrice = 200;
	int quantity = 2;
	int price = 400;

	char buf[128];
	char *pos = buf;
	memcpy( pos, keyword, sizeof( keyword ) );
	pos += sizeof( keyword );
	memcpy( pos, itemName, sizeof( itemName ) ); 
	pos += sizeof( itemName );
	memcpy( pos, &unitPrice, 4);
	pos += 4;
	memcpy( pos, &quantity, 4);
	pos += 4;
	memcpy( pos, &price, 4);
	pos += 4;

	int len = pos - buf;

	CPPUNIT_ASSERT( stream.write( buf, len ) == len ); 

	char bufRead[ 128 ];
	CPPUNIT_ASSERT( stream.read( bufRead, sizeof(bufRead) ) == len);

	pos = bufRead;

	char keywordRead[16];
	strcpy( keywordRead, pos );

	CPPUNIT_ASSERT( strcmp( keywordRead, keyword ) == 0 );

	pos += strlen( pos ) + 1;

	char itemNameRead[16];
	strcpy( itemNameRead, pos );

	CPPUNIT_ASSERT( strcmp( itemNameRead, itemName ) == 0 );

	pos += strlen( pos ) - 1;

	int unitPriceRead = *(int *)pos;

	CPPUNIT_ASSERT( unitPrice == 200 );

	pos += 4;

	int quantityRead = *(int *)pos;

	CPPUNIT_ASSERT( quantity == 2 );

	pos += 4;

	int priceRead = *(int *)pos;

	CPPUNIT_ASSERT( price == 400 );

	CloseHandle( mutex );
}

void CBinaryStreamTest::testReadWrite2()
{
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL ); 
	CPPUNIT_ASSERT( mutex != NULL );

	// Write 2 items, try to read 2 items
	CBinaryStream stream( mutex ) ;

	///////////////////
	char keyword[] = "Item";
	char teststr[] = "just a test";

	char buf[128];
	char *pos = buf;
	memcpy( pos, keyword, sizeof( keyword ) );
	pos += sizeof( keyword );
	memcpy( pos, teststr, sizeof( teststr ) );
	pos += sizeof( teststr );

	int len1 = pos - buf;

	CPPUNIT_ASSERT( stream.write( buf, len1 ) == len1 ); 

	///////////////////
	char keyword2[] = "Transaction";

	pos = buf;
	memcpy( pos, keyword2, sizeof( keyword2 ) );
	pos += sizeof( keyword2 );
	memcpy( pos, teststr, sizeof( teststr ) );
	pos += sizeof( teststr );

	int len2 = pos - buf;

	CPPUNIT_ASSERT( stream.write( buf, len2 ) == len2 ); 

	char bufRead[ 128 ];

	CPPUNIT_ASSERT( stream.read( bufRead, sizeof(bufRead) ) == len1 );
	CPPUNIT_ASSERT( strcmp( bufRead, keyword ) == 0 );

	CPPUNIT_ASSERT( stream.read( bufRead, sizeof(bufRead) ) == len2 );
	CPPUNIT_ASSERT( strcmp( bufRead, keyword2 ) == 0 );

	CloseHandle( mutex );
}

void CBinaryStreamTest::testReadWrite3()
{
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL ); 
	CPPUNIT_ASSERT( mutex != NULL );

	// Write 1 items, try to read 2 items
	CBinaryStream stream ( mutex ) ;

	char keyword[] = "Item";
	char teststr[] = "just a test";

	char buf[128];
	char *pos = buf;
	memcpy( pos, keyword, sizeof( keyword ) );
	pos += sizeof( keyword );
	memcpy( pos, teststr, sizeof( teststr ) );
	pos += sizeof( teststr );

	int len1 = pos - buf;

	CPPUNIT_ASSERT( stream.write( buf, len1 ) == len1 ); 

	char bufRead[ 128 ];

	CPPUNIT_ASSERT( stream.read( bufRead, sizeof(bufRead) ) == len1 );
	CPPUNIT_ASSERT( strcmp( bufRead, keyword ) == 0 );

	CPPUNIT_ASSERT( stream.read( bufRead, sizeof(bufRead) ) == 0 );

	CloseHandle( mutex );
}
