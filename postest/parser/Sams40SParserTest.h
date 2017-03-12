// Sams40SParserTest.h: interface for the CSams40SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMS40SPARSERTEST_H__778F87A0_36E0_4023_BD39_2EB2E8F6B5D5__INCLUDED_)
#define AFX_SAMS40SPARSERTEST_H__778F87A0_36E0_4023_BD39_2EB2E8F6B5D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CSams40SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CSams40SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testActualParse1 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetTime1();
	void testSetItem1();
	void testSetTotal1();
	void testSetPaymentType1();
	void testSetChange1();
	void testActualParse1();
	
	CSams40SParserTest();
	virtual ~CSams40SParserTest();

};

#endif // !defined(AFX_SAMS40SPARSERTEST_H__778F87A0_36E0_4023_BD39_2EB2E8F6B5D5__INCLUDED_)
