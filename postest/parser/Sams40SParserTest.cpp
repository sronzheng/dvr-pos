// Sams40SParserTest.cpp: implementation of the CSams40SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sams40SParserTest.h"

#include "Sams40SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CSams40SParserTest );

CSams40SParserTest::CSams40SParserTest()
{

}

CSams40SParserTest::~CSams40SParserTest()
{

}

void CSams40SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CSams40SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = "DATE 12/13/2005 FRI   TIME 14:15";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CSams40SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CSams40SParser parser( &receiver );

	// sale item. 
    // name:		PEPSI,	amount:		8.99 dollar  

	char buf[] = "PEPSI                      $8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "PEPSI" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CSams40SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CSams40SParser parser( &receiver );

	// total. 
	// 8.70 dollar  
	char buf[] = "TOTAL                     $8.70";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 70 );
}

void CSams40SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CSams40SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "CASH                      $6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CSams40SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CSams40SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHANGE                     $7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CSams40SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "DATE 03/18/2005 FRI   TIME 12:43";	// Time Gotten	
	char buf2[] = "APPLES                     $8.74";	// Item Gotten
	char buf3[] = "PEPSI                      $2.58";	// Item Gotten
	char buf4[] = "TOTAL                     $11.32";	// Total Gotten
	char buf5[] = "CASH                      $20.00";	// Payment Type Gotten
	char buf6[] = "CHANGE                     $8.68";	// Change Gotten

	CReceiverStub receiver;

	CSams40SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 74 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 58 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == SAMS40S_STATE_CHANGE_GOTTEN );
}
