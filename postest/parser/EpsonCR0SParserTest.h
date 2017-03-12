// EpsonCR0SParserTest.h: interface for the CEpsonCR0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPSONCR0SPARSERTEST_H__CBE5BFAF_A983_4B14_A791_9EE137756489__INCLUDED_)
#define AFX_EPSONCR0SPARSERTEST_H__CBE5BFAF_A983_4B14_A791_9EE137756489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CEpsonCR0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CEpsonCR0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetTime2 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItemVoided1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetPaymentType4 );
		CPPUNIT_TEST( testSetPaymentType5 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetSubtotal2 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTax2 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetChange1 );
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
	void testSetItemVoided1();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetPaymentType4();
	void testSetPaymentType5();
	void testSetSubtotal1();
	void testSetSubtotal2();
	void testSetTax1();
	void testSetTax2();
	void testSetTotal1();
	void testSetChange1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	
	CEpsonCR0SParserTest();
	virtual ~CEpsonCR0SParserTest();

};

#endif // !defined(AFX_EPSONCR0SPARSERTEST_H__CBE5BFAF_A983_4B14_A791_9EE137756489__INCLUDED_)
