// StreamReceiverTest.h: interface for the CStreamReceiverTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMRECEIVERTEST_H__A33C43D4_03BE_40C0_8AE4_D3EF4C11A250__INCLUDED_)
#define AFX_STREAMRECEIVERTEST_H__A33C43D4_03BE_40C0_8AE4_D3EF4C11A250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CStreamReceiverTest : public CPPUNIT_NS::TestFixture  
{
	CPPUNIT_TEST_SUITE( CStreamReceiverTest );
		CPPUNIT_TEST( testSetTime1 );
		CPPUNIT_TEST( testSetItem1 );
		CPPUNIT_TEST( testSetSubtotal1 );
		CPPUNIT_TEST( testSetTotal1 );
		CPPUNIT_TEST( testSetTax1 );
		CPPUNIT_TEST( testSetPaymentType1 );
		CPPUNIT_TEST( testSetPaymentType2 );
		CPPUNIT_TEST( testSetPaymentType3 );
		CPPUNIT_TEST( testSetPaymentType4 );
		CPPUNIT_TEST( testSetPaymentType5 );
		CPPUNIT_TEST( testSetChange1 );
		CPPUNIT_TEST( testSetItemVoided1 );
		CPPUNIT_TEST( testSetRefund1 );
		CPPUNIT_TEST( testSetTransactionVoided1 );
		CPPUNIT_TEST( testSetTransaction1 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testSetTime1();
	void testSetItem1();
	void testSetSubtotal1();
	void testSetTotal1();
	void testSetTax1();
	void testSetPaymentType1();
	void testSetPaymentType2();
	void testSetPaymentType3();
	void testSetPaymentType4();
	void testSetPaymentType5();
	void testSetChange1();
	void testSetItemVoided1();
	void testSetRefund1();
	void testSetTransactionVoided1();
	void testSetTransaction1();

	CStreamReceiverTest();
	virtual ~CStreamReceiverTest();

};

#endif // !defined(AFX_STREAMRECEIVERTEST_H__A33C43D4_03BE_40C0_8AE4_D3EF4C11A250__INCLUDED_)
