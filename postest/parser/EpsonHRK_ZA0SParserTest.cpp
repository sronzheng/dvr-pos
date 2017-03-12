// EpsonHRK_ZA0SParserTest.cpp: implementation of the CEpsonHRK_ZA0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EpsonHRK_ZA0SParserTest.h"

#include "EpsonHRK_ZA0SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CEpsonHRK_ZA0SParserTest );

CEpsonHRK_ZA0SParserTest::CEpsonHRK_ZA0SParserTest()
{

}

CEpsonHRK_ZA0SParserTest::~CEpsonHRK_ZA0SParserTest()
{

}

void CEpsonHRK_ZA0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 11:15
	char buf[] = "#01300 11:15 13-12-2005 004 CLK0626";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 11 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CEpsonHRK_ZA0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// sale item. 
    // name:		CASTLE DUMPIES 34,	amount:		8.99 dollar  

	char buf[] = " CASTLE DUMPIES 34 340    x12   8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "CASTLE DUMPIES 34" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CEpsonHRK_ZA0SParserTest::testSetItem2()
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// sale item. 
    // name:		CASTLE DUMPIES 34,	amount:		8.99 dollar  

	char buf[] = " BLACK LABEL DUMPI 340           8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "BLACK LABEL DUMPI" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CEpsonHRK_ZA0SParserTest::testSetItem3()
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// sale item. 
    // name:		CASTLE DUMPIES 34,	amount:		8.99 dollar  

	char buf[] = " CASTLE CANS 340   340          8.99-";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "CASTLE CANS 340" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -99 );
}

void CEpsonHRK_ZA0SParserTest::testSetItem4()
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// item voided
	// 2.00 dollar
	char buf[] = " BLACK LABEL CANS  340    x12   2.00-VD";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 0 );
}

void CEpsonHRK_ZA0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = " TOTAL                        R5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CEpsonHRK_ZA0SParserTest::testSetTotal2() 
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = " TOTAL       R5.82-";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == -5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == -82 );
}

void CEpsonHRK_ZA0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = " CASH                    6.35-";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == -6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == -35 );
}

void CEpsonHRK_ZA0SParserTest::testsetTransactionVoided1() 
{
	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	// transaction voied. 
	char buf[] = "TRANS 1300  TR VOID";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}

void CEpsonHRK_ZA0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "  WELCOME TO LIQUOR";					// Welcome Gotten
	char buf2[] = " CASTLE DUMPIES 34 340    x12   43.25";	// Item Gotten
	char buf3[] = "    TOTAL                      R43.25";	
	char buf4[] = " BLACK LABEL DUMPI 340           4.40";	// Item Gotten	
	char buf5[] = "    TOTAL                      R47.65";	
	char buf6[] = "    ROUNDED TO                R 47.60";
	char buf7[] = " TOTAL                         R47.60";	// Total Gotten
	char buf8[] = "   CASH                         47.60";	// Payment Type Gotten
	char buf9[] = " #01300 14:46 21-06-2004 004 CLK0626 ";	// Time Gotten

	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 43 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 25 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 43 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 25 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 4 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 40 );

	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 4 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 40 );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_DATE_TIME_GOTTEN );
}

void CEpsonHRK_ZA0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "  WELCOME TO LIQUOR";					// Welcome Gotten
	char buf2[] = " TRANS 1300  TR VOID";					// Transaction Voided Gotten
	char buf3[] = " #01300 14:46 21-06-2004 004 CLK0626 ";	// Time Gotten

	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_DATE_TIME_GOTTEN );
}

void CEpsonHRK_ZA0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "  WELCOME TO LIQUOR";					// Welcome Gotten
	char buf2[] = "      PAID OUT";							// Paid Out Gotten
	char buf3[] = " #01300 14:46 21-06-2004 004 CLK0626 ";	// Time Gotten

	CReceiverStub receiver;

	CEpsonHRK_ZA0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_PAID_OUT_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONHRK_ZA0S_STATE_DATE_TIME_GOTTEN );
}




