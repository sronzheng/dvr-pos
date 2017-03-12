// DresserWayne0SParserTest.cpp: implementation of the CDresserWayne0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DresserWayne0SParserTest.h"

#include "DresserWayne0SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CDresserWayne0SParserTest );

CDresserWayne0SParserTest::CDresserWayne0SParserTest()
{

}

CDresserWayne0SParserTest::~CDresserWayne0SParserTest()
{

}

void CDresserWayne0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// data & time
	// 05-27-2005, 10:28
	char buf[] = "0001e12s01t1 10:28:20am Mon 27 May 05";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 5 );
	CPPUNIT_ASSERT( receiver.m_day == 27 );
	CPPUNIT_ASSERT( receiver.m_hour == 10 );
	CPPUNIT_ASSERT( receiver.m_minute == 28 );
}

void CDresserWayne0SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// sale item. 
	// quantity:	1,				unit price:	5.95 dollar
    // name: Cooks Brut C,			amount:		5.95 dollar  

	char buf[] = "S2\x1B" "4 #08380404721    Cooks Brut C   5.95 T";
	
	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Cooks Brut C" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 95 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );
}

void CDresserWayne0SParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// sale item. 
	// quantity:	5,				unit price:	1.19 dollar
    // name: Cooks Brut C,			amount:		5.95 dollar  

	char buf1[] = "S2\x1B" "4                       5    @   1.19";
	char buf2[] = "S2\x1B" "4 #08380404721    Cooks Brut C   5.95 T";

	CPPUNIT_ASSERT( parser.setItemHead( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Cooks Brut C" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 19 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );
}

void CDresserWayne0SParserTest::testSetItem3() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// sale item. 
	// quantity:	1,				unit price:	0.29 dollar
    // name: GROCERY-TAX,			amount:		0.29 dollar  

	char buf[] = "S2\x1B" "4                 GROCERY-TAX    0.29 T";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "GROCERY-TAX" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 29 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 29 );
}

void CDresserWayne0SParserTest::testSetItem4() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// sale item. 

//	char buf1[] = "\x1B" "4                       5    @   1.19";
	char buf1[] = "S2\x1B" "4 #08380404721    Cooks Brut C   5.95 T";
	char buf2[] = "S2\x1B" "4 #08380404721    Cooks Brut C   5.95 T";
	char buf3[] = "0001e12s01t1 10:28:20am Mon 27 May 05";
	char buf4[] = "S2\x1B" "4 #08380404721    Cooks Brut C   5.95 T";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setTime( buf3 ) );
	CPPUNIT_ASSERT( parser.setItem( buf4 ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 2 );
}


void CDresserWayne0SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "ITEMS:  3        SUBTOTAL       6.08  ";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CDresserWayne0SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// tax. 
	// tax: 2.17 dollar
	char buf[] = "4                 SALES TAX      2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CDresserWayne0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "TOTAL     " "\x1B" "4         5.82  ";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CDresserWayne0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "Indoor Cash  6.35  ";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CDresserWayne0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// payment by bank card
	// amount:	5.82 dollar  
	char buf[] = "Indoor Debit  5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CDresserWayne0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "3            CHANGE DUE" "\x1B" "4  7.53  ";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CDresserWayne0SParserTest::testsetTransactionVoided1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// transaction voied. 
	char buf[] = " *** CANCELLED *** ";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}

void CDresserWayne0SParserTest::testSetItemVoided1() 
{
	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	// item voided
	// 2.01 dollar
	char buf[] = "         ITEM CORRECT   2.01- ";

	CPPUNIT_ASSERT( parser.setItemVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 1 );
}

void CDresserWayne0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "S2\x1B" "4 #07199030017    Coors Lt. 18  11.49 T";	// Item Gotten
	char buf2[] = "S2\x1B" "4 #02820000819    MARLB LT BX    3.49 T";	// Item Gotten
	char buf3[] = "S2\x1B" "4                  ITEM CORRECT  3.49- ";	// Item Voided Gotten
	char buf4[] = "S2\x1B" "4                        2    @  4.99  ";	// Item Head Gotten
	char buf5[] = "S2\x1B" "4 #08380404721    Cooks Brut C   9.98 T";	// Item Gotten
	char buf6[] = "S2\x1B" "4ITEMS:  5          SUBTOTAL    21.47  ";	// Subtotal Gotten
	char buf7[] = "S2\x1B" "4                   SALES TAX    1.87  ";	// Tax Gotten	
	char buf8[] = "S2\x1B" "4   3                TOTAL   4  23.34  ";	// Total Gotten
	char buf9[] = "S2\x1B" "4                 Indoor Cash   50.00  ";	// Payment Type Gotten
	char buf10[]= "S2\x1B" "4   3              CHANGE DUE4  26.66  ";	// Change Gotten	
	char buf11[]= "S2\x1B" "4 0044e11s01t1 12:01:32pm Mon 13 Oct 03";	// Time Gotten	

	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );

	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 11 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 49 );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 49 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 49 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 9 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 98 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN );
}

void CDresserWayne0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "S2\x1B" "4                        2    @  4.99  ";	// Item Head Gotten
	char buf2[] = "S2\x1B" "4 #08380404721    Cooks Brut C   9.98 T";	// Item Gotten
	char buf3[] = "S2\x1B" "4ITEMS:  5          SUBTOTAL     9.98  ";	// Subtotal Gotten
	char buf4[] = "S2\x1B" "4                   SALES TAX    0.87  ";	// Tax Gotten	
	char buf5[] = "S2\x1B" "4   3                TOTAL   4  10.85  ";	// Total Gotten
	char buf6[] = "S2\x1B" "3                    *** CANCELLED *** ";	// Transaction Voided Gotten
	char buf7[] = "S2\x1B" "4 0044e11s01t1 12:01:32pm Mon 13 Oct 03";	// Time Gotten	

	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN );
}

void CDresserWayne0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "S2\x1B" "4 #08380404721    Cooks Brut C   9.98 T";	// Item Gotten
	char buf2[] = "S2\x1B" "3                    *** CANCELLED *** ";	// Transaction Voided Gotten
	char buf3[] = "S2\x1B" "4 0044e11s01t1 12:01:32pm Mon 13 Oct 03";	// Time Gotten	

	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN );
}

void CDresserWayne0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "\x1B" "4  Safe Drop 100.00    Safe Drop 100.00";	// Safe Drop Gotten
	char buf2[] = "\x1B" "4 0044e11s01t1 12:01:32pm Mon 13 Oct 03";	// Time Gotten	

	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_SAFE_DROP_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN );
}

void CDresserWayne0SParserTest::testActualParse5()
{
	// state transform.

	char buf1[] = "\x1B" "4  PAID IN  55.31   PAID IN       55.31";	// Paid in Gotten
	char buf2[] = "\x1B" "4 0044e11s01t1 12:01:32pm Mon 13 Oct 03";	// Time Gotten	

	CReceiverStub receiver;

	CDresserWayne0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_PAID_IN_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN );
}



