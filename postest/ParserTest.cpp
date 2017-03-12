// ParserTest.cpp: implementation of the CParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParserTest.h"

#include "Parser.h"
#include "ParserStub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CParserTest );

CParserTest::CParserTest()
{

}

CParserTest::~CParserTest()
{

}

void CParserTest::testParse1()
{
	// handle multiple lines in one time.

	char buf[] = "REG          17-05-2003(SAT)  13:01 \r"		
				 "   1  FOOD                     1.00 \r";

	CParserStub parser;

	CPPUNIT_ASSERT( parser.parse( buf ) == buf );

	CPPUNIT_ASSERT( parser.m_lines.size() == 2 );
	CPPUNIT_ASSERT( parser.m_lines[0].compare( "REG          17-05-2003(SAT)  13:01 " ) == 0 );
	CPPUNIT_ASSERT( parser.m_lines[1].compare( "   1  FOOD                     1.00 " ) == 0 );
}

void CParserTest::testParse2()
{
	// handle one line in two times.

	char buf1[] = "REG          17-05-2003(SAT)";		
	char buf2[] =                             "  13:01 \r";		

	char buf[256];

	CParserStub parser;

	strcpy( buf, buf1 );
	CPPUNIT_ASSERT( parser.parse( buf ) == ( buf + strlen(buf1) ) );

	strcat( buf, buf2 );
	CPPUNIT_ASSERT( parser.parse( buf ) == buf );

	CPPUNIT_ASSERT( parser.m_lines.size() == 1 );
	CPPUNIT_ASSERT( parser.m_lines[0].compare( "REG          17-05-2003(SAT)  13:01 " ) == 0 );
}

void CParserTest::testGetMonth1()
{
	CParserStub parser;

	CPPUNIT_ASSERT( parser.getMonth( "JAN" ) == 1 );  
	CPPUNIT_ASSERT( parser.getMonth( "FEB" ) == 2 );  
	CPPUNIT_ASSERT( parser.getMonth( "MAR" ) == 3 );  
	CPPUNIT_ASSERT( parser.getMonth( "APR" ) == 4 );  
	CPPUNIT_ASSERT( parser.getMonth( "MAY" ) == 5 );  
	CPPUNIT_ASSERT( parser.getMonth( "JUN" ) == 6 );  
	CPPUNIT_ASSERT( parser.getMonth( "JUL" ) == 7 );  
	CPPUNIT_ASSERT( parser.getMonth( "AUG" ) == 8 );  
	CPPUNIT_ASSERT( parser.getMonth( "SEP" ) == 9 );  
	CPPUNIT_ASSERT( parser.getMonth( "OCT" ) == 10 );  
	CPPUNIT_ASSERT( parser.getMonth( "NOV" ) == 11 );  
	CPPUNIT_ASSERT( parser.getMonth( "DEC" ) == 12 );  

}

void CParserTest::testGetMoneyAmount1()
{
	char buf[] = "2.11";
	
	CParserStub parser;

	MONEY_AMOUNT amount;

	CPPUNIT_ASSERT( parser.getMoneyAmount( buf, amount ) != NULL );  

	CPPUNIT_ASSERT( amount.dollar == 2 );  
	CPPUNIT_ASSERT( amount.cent == 11 );  
}

void CParserTest::testGetMoneyAmount2()
{
	char buf[] = "0.11";
	
	CParserStub parser;

	MONEY_AMOUNT amount;

	CPPUNIT_ASSERT( parser.getMoneyAmount( buf, amount ) != NULL );  

	CPPUNIT_ASSERT( amount.dollar == 0 );  
	CPPUNIT_ASSERT( amount.cent == 11 );  
}

void CParserTest::testGetMoneyAmount3()
{
	char buf[] = ".11";
	
	CParserStub parser;

	MONEY_AMOUNT amount;

	CPPUNIT_ASSERT( parser.getMoneyAmount( buf, amount ) != NULL );  

	CPPUNIT_ASSERT( amount.dollar == 0 );  
	CPPUNIT_ASSERT( amount.cent == 11 );  
}

void CParserTest::testGetMoneyAmount4()
{
	char buf[] = "2.110";
	
	CParserStub parser;

	MONEY_AMOUNT amount;

	CPPUNIT_ASSERT( parser.getMoneyAmount( buf, amount ) != NULL );  

	CPPUNIT_ASSERT( amount.dollar == 2 );  
	CPPUNIT_ASSERT( amount.cent == 11 );  
}

void CParserTest::testGetNextDigit1()
{
	char buf[] = "abc 123 345";
	
	CParserStub parser;

	char *p = parser.nextDigit( buf );

	CPPUNIT_ASSERT( p != NULL );  
	CPPUNIT_ASSERT( *p == '1' );  
}

void CParserTest::testGetPreDigit1()
{
	char buf[] = "abc 123 345 f";
	
	CParserStub parser;

	char *pEnd = buf + strlen( buf );

	char *p = parser.preDigit( pEnd, buf );

	CPPUNIT_ASSERT( p != NULL );  
	CPPUNIT_ASSERT( *p == '5' );  
}
