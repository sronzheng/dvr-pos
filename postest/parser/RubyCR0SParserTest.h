// RubyCR0SParserTest.h: interface for the CRubyCR0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUBYCR0SPARSERTEST_H__3812FB4C_03CD_4548_B6B4_816D6DD1DE37__INCLUDED_)
#define AFX_RUBYCR0SPARSERTEST_H__3812FB4C_03CD_4548_B6B4_816D6DD1DE37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CRubyCR0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CRubyCR0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetItem5 );
		CPPUNIT_TEST( testSetItem6 );
		CPPUNIT_TEST( testSetItem7 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );
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
	void testSetSubtotal1();
	void testSetTax1();
	void testSetTotal1();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetChange1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	
	CRubyCR0SParserTest();
	virtual ~CRubyCR0SParserTest();

};

#endif // !defined(AFX_RUBYCR0SPARSERTEST_H__3812FB4C_03CD_4548_B6B4_816D6DD1DE37__INCLUDED_)
