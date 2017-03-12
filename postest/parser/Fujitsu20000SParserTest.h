// Fujitsu20000SParserTest.h: interface for the CFujitsu20000SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUJITSU20000SPARSERTEST_H__ECEB785A_3536_4344_B32C_4F6F2D237742__INCLUDED_)
#define AFX_FUJITSU20000SPARSERTEST_H__ECEB785A_3536_4344_B32C_4F6F2D237742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CFujitsu20000SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CFujitsu20000SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testsetTransactionVoided1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1();
	void testSetItem1();
	void testSetItem2();
	void testSetPaymentType1(); 
	void testSetChange1();
	void testSetSubtotal1();
	void testSetTax1();
	void testSetTotal1(); 
	void testsetTransactionVoided1(); 
	void testActualParse1();
	void testActualParse2();
	
	CFujitsu20000SParserTest();
	virtual ~CFujitsu20000SParserTest();

};

#endif // !defined(AFX_FUJITSU20000SPARSERTEST_H__ECEB785A_3536_4344_B32C_4F6F2D237742__INCLUDED_)
