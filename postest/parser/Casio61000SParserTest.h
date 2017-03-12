// Casio61000SParserTest.h: interface for the CCasio61000SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASIO61000SPARSERTEST_H__E0946338_F088_4EFC_B7B8_ADED9D22AAB4__INCLUDED_)
#define AFX_CASIO61000SPARSERTEST_H__E0946338_F088_4EFC_B7B8_ADED9D22AAB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CCasio61000SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CCasio61000SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testActualParse1 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1();
	void testSetItem1();
	void testSetTotal1();
	void testSetPaymentType1();
	void testActualParse1();

	CCasio61000SParserTest();
	virtual ~CCasio61000SParserTest();

};

#endif // !defined(AFX_CASIO61000SPARSERTEST_H__E0946338_F088_4EFC_B7B8_ADED9D22AAB4__INCLUDED_)
