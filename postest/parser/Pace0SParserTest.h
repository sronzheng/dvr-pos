// Pace0SParserTest.h: interface for the CPace0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACE0SPARSERTEST_H__07A41F6F_24D4_475F_B33D_217C7B927B2F__INCLUDED_)
#define AFX_PACE0SPARSERTEST_H__07A41F6F_24D4_475F_B33D_217C7B927B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CPace0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CPace0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetTime2 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetItem5 );
		CPPUNIT_TEST( testSetItem6 );
		CPPUNIT_TEST( testSetItem7 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetSubtotal2 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTax2 );
		CPPUNIT_TEST( testSetTax3 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTotal2 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetPaymentType4 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetTime1(); 
	void testSetTime2(); 
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItem4();
	void testSetItem5();
	void testSetItem6();
	void testSetItem7();
	void testSetSubtotal1();
	void testSetSubtotal2();
	void testSetTax1();
	void testSetTax2();
	void testSetTax3();
	void testSetTotal1(); 
	void testSetTotal2(); 
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetPaymentType4();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	
	CPace0SParserTest();
	virtual ~CPace0SParserTest();

};

#endif // !defined(AFX_PACE0SPARSERTEST_H__07A41F6F_24D4_475F_B33D_217C7B927B2F__INCLUDED_)
