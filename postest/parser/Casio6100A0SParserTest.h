// Casio6100A0SParserTest.h: interface for the CCasio6100A0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASIO6100A0SPARSERTEST_H__F69D23CC_B736_4437_BB28_582E4DA4C092__INCLUDED_)
#define AFX_CASIO6100A0SPARSERTEST_H__F69D23CC_B736_4437_BB28_582E4DA4C092__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CCasio6100A0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CCasio6100A0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1();
	void testSetItem1();
	void testSetTotal1();
	void testSetPaymentType1();
	void testSetChange1();
	void testActualParse1();
	void testActualParse2();
	
	CCasio6100A0SParserTest();
	virtual ~CCasio6100A0SParserTest();

};

#endif // !defined(AFX_CASIO6100A0SPARSERTEST_H__F69D23CC_B736_4437_BB28_582E4DA4C092__INCLUDED_)
