// Fujitsu1SParserTest.h: interface for the CFujitsu1SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUJITSU1SPARSERTEST_H__23894971_079E_4413_B04A_B0CBBFB299C5__INCLUDED_)
#define AFX_FUJITSU1SPARSERTEST_H__23894971_079E_4413_B04A_B0CBBFB299C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CFujitsu1SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CFujitsu1SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItemVoided1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testSetTransactionVoided1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );
		CPPUNIT_TEST( testActualParse5 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1(); 
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItemVoided1();
	void testSetTransactionVoided1();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetSubtotal1();
	void testSetTax1();
	void testSetTotal1();
	void testSetChange1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	void testActualParse5();
	
	CFujitsu1SParserTest();
	virtual ~CFujitsu1SParserTest();

};

#endif // !defined(AFX_FUJITSU1SPARSERTEST_H__23894971_079E_4413_B04A_B0CBBFB299C5__INCLUDED_)
