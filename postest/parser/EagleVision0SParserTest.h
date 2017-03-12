// CasioParserTest.h: interface for the CEagleVision0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASIOPARSERTEST_H__1F5E8185_96BF_4CBB_953D_5F20DF74F01A__INCLUDED_)
#define AFX_CASIOPARSERTEST_H__1F5E8185_96BF_4CBB_953D_5F20DF74F01A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CEagleVision0SParserTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CEagleVision0SParserTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetItem2 );
		CPPUNIT_TEST( testSetItem3 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetSubtotal2 );
		CPPUNIT_TEST( testSetSubtotal3 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTotal2 );
		CPPUNIT_TEST( testSetTotal3 );
		CPPUNIT_TEST( testSetCashOfPayment1 );
		CPPUNIT_TEST( testSetCashOfPayment2 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetItemVoided1 );
		CPPUNIT_TEST( testSetItemVoided2 );
		CPPUNIT_TEST( testSetRefund1 );
		CPPUNIT_TEST( testSetRefund2 );
		CPPUNIT_TEST( testsetTransactionVoided1 );
		CPPUNIT_TEST( testActualParse1 );
		CPPUNIT_TEST( testActualParse2 );
		CPPUNIT_TEST( testActualParse3 );
		CPPUNIT_TEST( testActualParse4 );

	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1(); 
	void testSetItem1(); 
	void testSetItem2(); 
	void testSetItem3(); 
	void testSetSubtotal1();
	void testSetSubtotal2();
	void testSetSubtotal3();
	void testSetTotal1();
	void testSetTotal2();
	void testSetTotal3();
	void testSetCashOfPayment1(); 	
	void testSetCashOfPayment2(); 	
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetItemVoided1(); 
	void testSetItemVoided2(); 
	void testSetRefund1(); 
	void testSetRefund2(); 
	void testsetTransactionVoided1();
	void testSetCardType1(); 
	void testActualParse1(); 
	void testActualParse2(); 
	void testActualParse3(); 
	void testActualParse4(); 

	CEagleVision0SParserTest();
	virtual ~CEagleVision0SParserTest();
};

#endif // !defined(AFX_CASIOPARSERTEST_H__1F5E8185_96BF_4CBB_953D_5F20DF74F01A__INCLUDED_)
