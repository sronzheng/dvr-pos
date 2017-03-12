// Fujitsu20000SParserTest.cpp: implementation of the CFujitsu20000SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fujitsu20000SParserTest.h"

#include "Fujitsu20000SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CFujitsu20000SParserTest );

CFujitsu20000SParserTest::CFujitsu20000SParserTest()
{

}

CFujitsu20000SParserTest::~CFujitsu20000SParserTest()
{

}

void CFujitsu20000SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// data & time
	// 05-27-2005, 10:28
	char buf[] = "  C0202    #0039    10:28:42     27MAY2005 ";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 5 );
	CPPUNIT_ASSERT( receiver.m_day == 27 );
	CPPUNIT_ASSERT( receiver.m_hour == 10 );
	CPPUNIT_ASSERT( receiver.m_minute == 28 );
}

void CFujitsu20000SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// sale item. 
    // name: MEAT,			amount:		5.95 dollar  

	char buf[] = "    MEAT                         $5.95   F";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "MEAT" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );
}

void CFujitsu20000SParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// sale item. 

	char buf1[] = "    MEAT                         $5.95   F";
	char buf2[] = "    MEAT                         $5.95   F";
	char buf3[] = "  C0202    #0039    10:28:42     27MAY2005 ";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setTime( buf3 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == false );
}

void CFujitsu20000SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = " CASH                       $6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CFujitsu20000SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = " CHANGE                     $7.53 ";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CFujitsu20000SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = " SUB TOTAL                  $6.08 ";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CFujitsu20000SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// tax. 
	// tax: 2.17 dollar
	char buf[] = " TOTAL TAX                  $2.17 ";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CFujitsu20000SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = " TOTAL                      $5.82 ";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CFujitsu20000SParserTest::testsetTransactionVoided1() 
{
	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	// transaction voied. 
	char buf[] = "* VOID TRANSACTION *";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}

void CFujitsu20000SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "    MEAT                         $0.10   F";	// Item Gotten
	char buf2[] = "    PRODUCE                      $0.15   F";	// Item Gotten
	char buf3[] = "          BALANCE DUE              $0.50  ";	// Balance Due Gotten
	char buf4[] = "  E       CASH                     $1.00  ";	// Payment Type Gotten
	char buf5[] = "      CHANGE                     $0.50    ";	// Change Gotten
	char buf6[] = "      SUB TOTAL                  $0.50    ";	// Total Gotten
	char buf7[] = "      TOTAL TAX                  $0.00    ";	// Tax Gotten	
	char buf8[] = "      TOTAL                      $0.50    ";	// Total Gotten
	char buf9[] = "  C0202    #0039    16:17:42     4OCT2005 ";	// Time Gotten

	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 10 );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 15 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_BALANCE_DUE_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_TAX_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_DATE_TIME_GOTTEN );
}

void CFujitsu20000SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "    MEAT                         $0.10   F";	// Item Gotten
	char buf2[] = " * VOID TRANSACTION *";						// Transaction Voided Gotten
	char buf3[] = "  C0202    #0039    16:17:42     4OCT2005 ";	// Time Gotten

	CReceiverStub receiver;

	CFujitsu20000SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU20000S_STATE_DATE_TIME_GOTTEN );
}
