// SharpParserTest.h: interface for the CSharpParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHARPPARSERTEST_H__D3732FA2_6A55_460B_BD9F_1E93AB4F9CAC__INCLUDED_)
#define AFX_SHARPPARSERTEST_H__D3732FA2_6A55_460B_BD9F_1E93AB4F9CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CSharpParserTest : public CPPUNIT_NS::TestFixture  
{
	CPPUNIT_TEST_SUITE( CSharpParserTest );
		CPPUNIT_TEST( testSetDate1 );
		CPPUNIT_TEST( testSetDate2 );
		CPPUNIT_TEST( testSetDate3 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetItem5 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTax2 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );
		CPPUNIT_TEST( testActualParse5 );
		CPPUNIT_TEST( testActualParse6 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetDate1();
	void testSetDate2();
	void testSetDate3();
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItem4();
	void testSetItem5();
	void testSetSubtotal1();
	void testSetTax1();
	void testSetTax2();
	void testSetTotal1();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetChange1();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	void testActualParse4();
	void testActualParse5();
	void testActualParse6();

	CSharpParserTest();
	virtual ~CSharpParserTest();

};

#endif // !defined(AFX_SHARPPARSERTEST_H__D3732FA2_6A55_460B_BD9F_1E93AB4F9CAC__INCLUDED_)
