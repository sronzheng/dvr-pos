// ParserTest.h: interface for the CParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSERTEST_H__DD135BF5_83E6_41C1_9545_C6C5D4F0C9A2__INCLUDED_)
#define AFX_PARSERTEST_H__DD135BF5_83E6_41C1_9545_C6C5D4F0C9A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CParserTest );
		CPPUNIT_TEST( testParse1 );
		CPPUNIT_TEST( testParse2 );
		CPPUNIT_TEST( testGetMonth1 );
		CPPUNIT_TEST( testGetMoneyAmount1 );
		CPPUNIT_TEST( testGetMoneyAmount2 );
		CPPUNIT_TEST( testGetMoneyAmount3 );
		CPPUNIT_TEST( testGetMoneyAmount4 );
		CPPUNIT_TEST( testGetNextDigit1 );
		CPPUNIT_TEST( testGetPreDigit1 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testParse1();
	void testParse2();
	void testGetMonth1();
	void testGetMoneyAmount1();
	void testGetMoneyAmount2();
	void testGetMoneyAmount3();
	void testGetMoneyAmount4();
	void testGetNextDigit1();
	void testGetPreDigit1();

	CParserTest();
	virtual ~CParserTest();

};

#endif // !defined(AFX_PARSERTEST_H__DD135BF5_83E6_41C1_9545_C6C5D4F0C9A2__INCLUDED_)
