// EpsonIStore0SParserTest.h: interface for the CEpsonIStore0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPSONISTORE0SPARSERTEST_H__65D305AB_BB65_4186_ADDC_46559F2C478D__INCLUDED_)
#define AFX_EPSONISTORE0SPARSERTEST_H__65D305AB_BB65_4186_ADDC_46559F2C478D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CEpsonIStore0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CEpsonIStore0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetItem5 );
		CPPUNIT_TEST( testSetItem6 );
		CPPUNIT_TEST( testSetItemVoided1 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetSubtotal2 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTotal2 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testSetChange2 );
		CPPUNIT_TEST( testsetTransactionVoided1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );
		CPPUNIT_TEST( testActualParse5 );
		CPPUNIT_TEST( testActualParse6 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1();
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItem4();
	void testSetItem5();
	void testSetItem6();
	void testSetItemVoided1(); 
	void testSetSubtotal1();
	void testSetSubtotal2();
	void testSetTotal1();
	void testSetTotal2();
	void testSetPaymentType1();
	void testSetChange1();
	void testSetChange2();
	void testsetTransactionVoided1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	void testActualParse5();
	void testActualParse6();
	
	CEpsonIStore0SParserTest();
	virtual ~CEpsonIStore0SParserTest();

};

#endif // !defined(AFX_EPSONISTORE0SPARSERTEST_H__65D305AB_BB65_4186_ADDC_46559F2C478D__INCLUDED_)
