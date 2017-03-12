// StarTSP7000SParserTest.h: interface for the CStarTSP7000SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STARTSP7000SPARSERTEST_H__F34ED746_B602_493D_81D4_7F35106ED9C5__INCLUDED_)
#define AFX_STARTSP7000SPARSERTEST_H__F34ED746_B602_493D_81D4_7F35106ED9C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CStarTSP7000SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CStarTSP7000SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTotal2 );
		CPPUNIT_TEST( testActualParse1 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1();
	void testSetItem1();
	void testSetTotal1(); 
	void testSetTotal2(); 
	void testActualParse1();
	
	CStarTSP7000SParserTest();
	virtual ~CStarTSP7000SParserTest();

};

#endif // !defined(AFX_STARTSP7000SPARSERTEST_H__F34ED746_B602_493D_81D4_7F35106ED9C5__INCLUDED_)
