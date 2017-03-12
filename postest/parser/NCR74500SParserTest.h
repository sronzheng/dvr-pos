// NCR74500SParserTest.h: interface for the CNCR74500SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCR74500SPARSERTEST_H__AD60BC70_AFAD_4559_A528_18404E245E14__INCLUDED_)
#define AFX_NCR74500SPARSERTEST_H__AD60BC70_AFAD_4559_A528_18404E245E14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CNCR74500SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CNCR74500SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetTime2 );
		CPPUNIT_TEST( testSetItem1 );
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
		CPPUNIT_TEST( testActualParse5 );
		CPPUNIT_TEST( testActualParse6 );
		CPPUNIT_TEST( testsetTransactionVoided1 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1(); 
	void testSetTime2(); 
	void testSetItem1();
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
	void testActualParse5();
	void testActualParse6();
	void testsetTransactionVoided1();
	
	CNCR74500SParserTest();
	virtual ~CNCR74500SParserTest();

};

#endif // !defined(AFX_NCR74500SPARSERTEST_H__AD60BC70_AFAD_4559_A528_18404E245E14__INCLUDED_)
