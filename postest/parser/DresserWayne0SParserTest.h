// DresserWayne0SParserTest.h: interface for the CDresserWayne0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRESSERWAYNE0SPARSERTEST_H__F03563BD_6807_4E33_8113_392F43A2EAAA__INCLUDED_)
#define AFX_DRESSERWAYNE0SPARSERTEST_H__F03563BD_6807_4E33_8113_392F43A2EAAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <cppunit/extensions/HelperMacros.h>

class CDresserWayne0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CDresserWayne0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testsetTransactionVoided1 );
		CPPUNIT_TEST( testSetItemVoided1 );
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
	void testSetItem4();
	void testSetSubtotal1();
	void testSetTax1();
	void testSetTotal1();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetChange1();
	void testsetTransactionVoided1();
	void testSetItemVoided1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	void testActualParse5();
	
	CDresserWayne0SParserTest();
	virtual ~CDresserWayne0SParserTest();

};

#endif // !defined(AFX_DRESSERWAYNE0SPARSERTEST_H__F03563BD_6807_4E33_8113_392F43A2EAAA__INCLUDED_)
