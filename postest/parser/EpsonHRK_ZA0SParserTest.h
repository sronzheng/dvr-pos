// EpsonHRK_ZA0SParserTest.h: interface for the CEpsonHRK_ZA0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPSONHRK_ZA0SPARSERTEST_H__1E2B9CD9_CD82_43AC_9D00_1764217C5007__INCLUDED_)
#define AFX_EPSONHRK_ZA0SPARSERTEST_H__1E2B9CD9_CD82_43AC_9D00_1764217C5007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CEpsonHRK_ZA0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CEpsonHRK_ZA0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTotal2 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testsetTransactionVoided1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetTime1();
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItem4();
	void testSetTotal1();
	void testSetTotal2();
	void testSetPaymentType1();
	void testsetTransactionVoided1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	
	CEpsonHRK_ZA0SParserTest();
	virtual ~CEpsonHRK_ZA0SParserTest();

};

#endif // !defined(AFX_EPSONHRK_ZA0SPARSERTEST_H__1E2B9CD9_CD82_43AC_9D00_1764217C5007__INCLUDED_)
