// Panda0SParserTest.h: interface for the CPanda0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANDA0SPARSERTEST_H__9E7AE5B2_75F3_4BB7_A5AC_1D1B93E5398D__INCLUDED_)
#define AFX_PANDA0SPARSERTEST_H__9E7AE5B2_75F3_4BB7_A5AC_1D1B93E5398D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CPanda0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CPanda0SParserTest );
		CPPUNIT_TEST( testTestTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetItem4 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetTax2 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTotal2 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testSetChange2 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testTestTime1();
	void testSetItem1();
	void testSetItem2();
	void testSetItem3();
	void testSetItem4();
	void testSetTax1();
	void testSetTax2();
	void testSetTotal1();
	void testSetTotal2();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetChange1();
	void testSetChange2();
	void testActualParse1();
	void testActualParse2();
	void testActualParse3();
	
	CPanda0SParserTest();
	virtual ~CPanda0SParserTest();

};

#endif // !defined(AFX_PANDA0SPARSERTEST_H__9E7AE5B2_75F3_4BB7_A5AC_1D1B93E5398D__INCLUDED_)
