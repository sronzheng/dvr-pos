// Gilbarco0SParserTest.h: interface for the CGilbarco0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GILBARCO0SPARSERTEST_H__45C9D7EB_AA7B_4EDC_962F_CF12D1EC9A01__INCLUDED_)
#define AFX_GILBARCO0SPARSERTEST_H__45C9D7EB_AA7B_4EDC_962F_CF12D1EC9A01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CGilbarco0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CGilbarco0SParserTest );
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
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
	CPPUNIT_TEST_SUITE_END();

public:
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
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	
	CGilbarco0SParserTest();
	virtual ~CGilbarco0SParserTest();

};

#endif // !defined(AFX_GILBARCO0SPARSERTEST_H__45C9D7EB_AA7B_4EDC_962F_CF12D1EC9A01__INCLUDED_)
