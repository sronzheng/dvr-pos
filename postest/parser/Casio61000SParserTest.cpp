// Casio61000SParserTest.cpp: implementation of the CCasio61000SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Casio61000SParserTest.h"

#include "Casio61000SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CCasio61000SParserTest );

CCasio61000SParserTest::CCasio61000SParserTest()
{

}

CCasio61000SParserTest::~CCasio61000SParserTest()
{

}

void CCasio61000SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CCasio61000SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = "REG  12-13-2005 14:15   ";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CCasio61000SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CCasio61000SParser parser( &receiver );

	// sale item. 
    // name:		FOOD,	amount:		8.99 dollar  

	char buf[] = "	 1 CAKE            $8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "CAKE" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CCasio61000SParserTest::testSetTotal1()
{
	CReceiverStub receiver;

	CCasio61000SParser parser( &receiver );

	// total. 
	// 8.70 dollar  
	char buf[] = "   TL            $ 8 .7 0  ";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 70 );
}

void CCasio61000SParserTest::testSetPaymentType1()
{
	CReceiverStub receiver;

	CCasio61000SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "   CASH            $6.35 ";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CCasio61000SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "REG  05-30-2003 20:45   ";	// Time Gotten		
	char buf2[] = "C 01     MC#01    000509";
	char buf3[] = "                        ";
	char buf4[] = " 1 CAKE            $0.14";	// Item Gotten
	char buf5[] = " 1 MILK            $0.21";	// Item Gotten
	char buf6[] = "   TL         $ 0 .3 5  ";	// Total Gotten
	char buf7[] = "   CASH            $2.28";	// Payment Type Gotten

	CReceiverStub receiver;

	CCasio61000SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 14 );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 21 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == CASIO61000S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
}

