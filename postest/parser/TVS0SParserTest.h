// TVS0SParserTest.h: interface for the CTVS0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TVS0SPARSERTEST_H__2004650B_7B26_4728_9EAB_57DE52F709F1__INCLUDED_)
#define AFX_TVS0SPARSERTEST_H__2004650B_7B26_4728_9EAB_57DE52F709F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CTVS0SParserTest : public CPPUNIT_NS::TestFixture  
{
	CPPUNIT_TEST_SUITE( CTVS0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetItem5 );
		CPPUNIT_TEST( testSetItem6 );
		CPPUNIT_TEST( testSetItem7 );
		CPPUNIT_TEST( testSetItem8 );
		CPPUNIT_TEST( testSetItem9 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );
		CPPUNIT_TEST( testParse1 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetTime1();
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItem4();
	void testSetItem5();
	void testSetItem6();
	void testSetItem7();
	void testSetItem8();
	void testSetItem9();
	void testSetTax1();
	void testSetTotal1();
	void testSetPaymentType1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	void testParse1();

	CTVS0SParserTest();
	virtual ~CTVS0SParserTest();

};

#endif // !defined(AFX_TVS0SPARSERTEST_H__2004650B_7B26_4728_9EAB_57DE52F709F1__INCLUDED_)
