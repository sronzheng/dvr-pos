// Fujitsu1SParserTest.cpp: implementation of the CFujitsu1SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fujitsu1SParserTest.h"

#include "Fujitsu1SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CFujitsu1SParserTest );

CFujitsu1SParserTest::CFujitsu1SParserTest()
{

}

CFujitsu1SParserTest::~CFujitsu1SParserTest()
{

}

void CFujitsu1SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// data & time
	// 12-13-2005, 11:15
	char buf1[] = "e2s82t1   12/13/05 11:15:27";
	char buf2[] = "SWISHERSWEET        1.38T";
	char buf3[] = "Transaction Complete";

	CPPUNIT_ASSERT( parser.setTime( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_year == 0 );
	CPPUNIT_ASSERT( parser.m_timeInformed == false );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( parser.m_timeInformed == true );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 11 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );

	CPPUNIT_ASSERT( parser.setEndFlag( buf3 ) );
	CPPUNIT_ASSERT( parser.m_timeInformed == false );
}

void CFujitsu1SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// sale item. 
	// quantity:	1,				unit price:	1.38 dollar
    // name: SWISHERSWEET,			amount:		1.38 dollar  

	char buf[] = "SWISHERSWEET        1.38T";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "SWISHERSWEET" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 38 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 38 );
}

void CFujitsu1SParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// sale item. 
	// quantity:	2,				unit price:	0.69 dollar
    // name: SWISHERSWEET,			amount:		1.38 dollar  

	char buf[] = "SWISHERSWEET       2 @   0.69    1.38T";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "SWISHERSWEET" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 69 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 38 );
}

void CFujitsu1SParserTest::testSetItem3() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// sale item. 
	// quantity:	1,				unit price:	1.38 dollar
    // name: SWISHERSWEET,			amount:		1.38 dollar  

	char buf[] = "REESES CUPS                    --1.38";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "REESES CUPS" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 38 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 38 );
}

void CFujitsu1SParserTest::testSetItemVoided1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// item voided, amount with dollar part.
	// 1.19 dollar
	char buf[] = "*VOID 24 OZ COFFEE*             -1.19T";

	CPPUNIT_ASSERT( parser.setItemVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 19 );
}

void CFujitsu1SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "Cash                             6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CFujitsu1SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// payment by gift certificate
	// amount:	5.82 dollar  
	char buf[] = "Credit Card                      5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CFujitsu1SParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// payment by bank card
	// amount:	5.82 dollar  
	char buf[] = "Debit Card                       5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CFujitsu1SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "Subtotal:               6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CFujitsu1SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// tax. 
	// tax: 2.17 dollar
	char buf[] = "Sales TAX:                       2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CFujitsu1SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "Total:               5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CFujitsu1SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "Cash Change Due:                7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CFujitsu1SParserTest::testSetTransactionVoided1() 
{
	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	// transaction voied. 
	char buf[] = "CANCELED";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}


void CFujitsu1SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "e2s82t1   03/03/04 06:13:58";			// Time Gotten	
	char buf2[] = "No Sale";								// Time Expected
	char buf3[] = "e2s82t1   03/03/04 06:14:00";			// Time Gotten	
	char buf4[] = "SWISHERSWEET       2 @   5.69  11.38T";	// Item Gotten
	char buf5[] = "e2s82t1   03/03/04 06:14:02";	
	char buf6[] = "HST HOHO                         1.19";	// Item Gotten
	char buf7[] = "e2s82t1   03/03/04 06:14:02";	
	char buf8[] = "*VOID HST HOHO*                -1.19T";	// Item Voided Gotten
	char buf9[] = "e2s82t1   03/03/04 06:14:06";	
	char buf10[] = "Cash                           10.00";	// Payment Type Gotten
	char buf11[] = "e2s82t1   03/03/04 06:14:06";	
	char buf12[] = "Debit Card                     10.00";	// Payment Type Gotten
	char buf13[] = "e2s82t1   03/03/04 06:14:06";	
	char buf14[] = "Subtotal:                       11.38";	// Subtotal Gotten
	char buf15[] = "e2s82t1   03/03/04 06:14:06";			
	char buf16[] = "Sales TAX:                       0.10";	// Tax Gotten	
	char buf17[] = "e2s82t1   03/03/04 06:14:06";	
	char buf18[] = "Total:                         11.48";	// Total Gotten
	char buf19[] = "e2s82t1   03/03/04 06:14:06";	
	char buf20[] ="Cash Change Due:                 8.52";	// Change Gotten	
	char buf21[] = "e2s82t1   03/03/04 06:14:06";	
	char buf22[] = "Transaction ID#95749";					// End Flag Gotten

	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_EXPECTED );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 11 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 38 );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 11 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 38 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 19 );

	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 19 );

	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf11);
	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );

	parser.parseLine( buf13 );
	parser.parseLine( buf14 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf15 );
	parser.parseLine( buf16 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_TAX_GOTTEN );

	parser.parseLine( buf17 );
	parser.parseLine( buf18 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf19 );
	parser.parseLine( buf20 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf21 );
	parser.parseLine( buf22 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_END_FLAG_GOTTEN );
}

void CFujitsu1SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "e2s82t1   03/03/04 06:59:01";			// Time Gotten	
	char buf2[] = "PROPEL WATER                     1.39";	// Item Gotten
	char buf3[] = "e2s82t1   03/03/04 06:59:45";	
	char buf4[] = "CANCELED";								// Transaction Voided Gotten
	char buf5[] = "e2s82t1   03/03/04 06:59:45";	
	char buf6[] = "Subtotal:                       1.39";	
	char buf7[] = "e2s82t1   03/03/04 06:59:45";			
	char buf8[] = "Sales TAX:                      0.10";	
	char buf9[] = "e2s82t1   03/03/04 06:59:45";	
	char buf10[] = "Total:                          1.49";	
	char buf11[] = "e2s82t1   03/03/04 06:59:45";	
	char buf12[] = "Transaction ID#95800";					// End Flag Gotten

	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf11);
	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_END_FLAG_GOTTEN );
}

void CFujitsu1SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "e2s82t1   03/03/04 05:58:27";			// Time Gotten	
	char buf2[] = "Safe Loan";								// Safe Loan Gotten
	char buf3[] = "e2s82t1   03/03/04 05:58:27";	
	char buf4[] = "Enter $$$, Tender, Press <enter>";						
	char buf5[] = "e2s82t1   03/03/04 05:58:29";	
	char buf6[] = "Cash                            10.00";	
	char buf7[] = "e2s82t1   03/03/04 05:58:29";			
	char buf8[] = "Transaction Complete";					// End Flag Gotten

	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SAFE_LOAN_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_END_FLAG_GOTTEN );
}

void CFujitsu1SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "e2s82t1   03/03/04 05:58:27";			// Time Gotten	
	char buf2[] = "Paid Out";								// Safe Loan Gotten
	char buf3[] = "e2s82t1   03/03/04 05:58:27";	
	char buf4[] = "Enter $$$, Tender, Press <enter>";						
	char buf5[] = "e2s82t1   03/03/04 05:58:29";	
	char buf6[] = "Cash                            10.00";	
	char buf7[] = "e2s82t1   03/03/04 05:58:29";			
	char buf8[] = "Transaction Complete";					// End Flag Gotten

	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_PAID_OUT_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_END_FLAG_GOTTEN );
}

void CFujitsu1SParserTest::testActualParse5()
{
	// state transform.

	char buf1[] = "e2s82t1   03/03/04 05:58:27";			// Time Gotten	
	char buf2[] = "Safe Drop";								// Safe Loan Gotten
	char buf3[] = "e2s82t1   03/03/04 05:58:27";	
	char buf4[] = "Enter $$$, Tender, Press <enter>";						
	char buf5[] = "e2s82t1   03/03/04 05:58:29";	
	char buf6[] = "Cash                            10.00";	
	char buf7[] = "e2s82t1   03/03/04 05:58:29";			
	char buf8[] = "Transaction Complete";					// End Flag Gotten

	CReceiverStub receiver;

	CFujitsu1SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_SAFE_DROP_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == FUJITSU1S_STATE_END_FLAG_GOTTEN );
}



