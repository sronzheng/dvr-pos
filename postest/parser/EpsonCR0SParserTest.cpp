// EpsonCR0SParserTest.cpp: implementation of the CEpsonCR0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EpsonCR0SParserTest.h"

#include "EpsonCR0SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CEpsonCR0SParserTest );

CEpsonCR0SParserTest::CEpsonCR0SParserTest()
{

}

CEpsonCR0SParserTest::~CEpsonCR0SParserTest()
{

}

void CEpsonCR0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 11:15
	char buf[] = "=" "\x1" "12/13/2005         11:15 AM         000190";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 11 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CEpsonCR0SParserTest::testSetTime2()
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 16:15
	char buf[] = "=" "\x1" "12/13/2005         04:15 PM         000190";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 16 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CEpsonCR0SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// sale item. 
	// quantity:	1,				unit price:	5.95 dollar
    // name: WRIG ALPINE CH EACH,	amount:		5.95 dollar  

	char buf[] = "=" "\x1" "06490000057   WRIG ALPINE CH EACH     -5.95";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "WRIG ALPINE CH EACH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 95 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -95 );
}

void CEpsonCR0SParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// sale item. 
	// quantity:	5,				unit price:	1.19 dollar
    // name: WRIG ALPINE CH EACH,	amount:		5.95 dollar  

	char buf1[] = "=" "\x1" "            5  @ 1.1900                   ";
	char buf2[] = "=" "\x1" "06490000057   WRIG ALPINE CH EACH     5.95";

	CPPUNIT_ASSERT( parser.setItemHead( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "WRIG ALPINE CH EACH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 19 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );
}

void CEpsonCR0SParserTest::testSetItem3() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// sale item. 
	// quantity:	1,				unit price:	0.29 dollar
    // name: Coupon Store,			amount:		-0.29 dollar  

	char buf[] = "=" "\x1" "Coupon Store                         -0.29";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Coupon Store" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 29 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -29 );
}

void CEpsonCR0SParserTest::testSetItemVoided1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// item voided, amount with dollar part.
	// 2.01 dollar
	char buf[] = "00FFFF=" "\x1" "REMOVED       Coupon Store            2.01";

	CPPUNIT_ASSERT( parser.setItemVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 1 );
}

void CEpsonCR0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "00FFFF=" "Cash                     6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CEpsonCR0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// payment by gift certificate
	// amount:	5.82 dollar  
	char buf[] = "00FFFF=" "Gift Certificates                     5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CEpsonCR0SParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// payment by credit card
	// amount:	5.82 dollar  
	char buf[] = "00FFFF=" "Visa Cash                       5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CEpsonCR0SParserTest::testSetPaymentType4() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// payment by credit card
	// amount:	5.82 dollar  
	char buf[] = "00FFFF=" "Master Card                           5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CEpsonCR0SParserTest::testSetPaymentType5() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// payment by bank card
	// amount:	5.82 dollar  
	char buf[] = "00FFFF=" "Debit                                5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CEpsonCR0SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "00FFFF=" "SUBTOTAL                              6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CEpsonCR0SParserTest::testSetSubtotal2() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "00FFFF=" "SUBTOTAL                              -6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == -6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == -8 );
}

void CEpsonCR0SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// tax. 
	// tax: 2.17 dollar
	char buf[] = "00FFFF=" "GST                                   2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CEpsonCR0SParserTest::testSetTax2() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// tax. 
	// tax: 2.17 dollar
	char buf[] = "00FFFF=" "PST                                   -2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == -2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == -17 );
}

void CEpsonCR0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "00FFFF=" "TOTAL                                -5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == -5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == -82 );
}

void CEpsonCR0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "00FFFF=" "CHANGE                          7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CEpsonCR0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "=" "\x1" "12/11/2003        10:32 AM          000190";// Time Gotten	
	char buf2[] = "=" "\x1" "06490000046   WRIG ALPINE ME EACH    11.90";// Item Voided Gotten
	char buf3[] = "=" "\x1" "06490000056   WRIG ALPINE HO EACH     9.52";// Item Gotten
	char buf4[] = "=" "\x1" "             5  @ 1.1900                  ";// Item Head Gotten
	char buf5[] = "=" "\x1" "06490000057   WRIG ALPINE CH EACH     5.95";// Item Gotten
	char buf6[] = "00FFFF=" "Cash                                 50.00";// Payment Type Gotten
	char buf7[] = "00FFFF=" "SUBTOTAL                             27.37";// Subtotal Gotten
	char buf8[] = "00FFFF=" "PST                                   2.19";// Tax Gotten	
	char buf9[] = "00FFFF=" "GST                                   1.92";// Tax Gotten	
	char buf10[] = "00FFFF=" "TOTAL                               31.48";// Total Gotten
	char buf11[] = "00FFFF=" "TOTAL TENDERED                      50.00";	
	char buf12[] = "00FFFF=" "CHANGE                              18.52";// Change Gotten	
	char buf13[] = "Please Come Again!                                 ";// Thank Gotten

	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 11 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 90 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 9 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 52 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_TAX_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 19 );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_TAX_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_tax.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 92 );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_THANK_GOTTEN );
}

void CEpsonCR0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "=" "\x1" "12/11/2003        10:32 AM          000190";// Time Gotten	
	char buf2[] = "=" "\x1" "             5  @ 1.1900                  ";// Item Head Gotten
	char buf3[] = "=" "\x1" "06490000057   WRIG ALPINE CH EACH     5.95";// Item Gotten
	char buf4[] = "00FFFF=" "Cash                                 5.95";// Payment Type Gotten
	char buf5[] = "Please Come Again!                        ";// Thank Gotten

	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_THANK_GOTTEN );
}

void CEpsonCR0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "=" "\x1" "12/11/2003        10:32 AM          000190";// Time Gotten	
	char buf2[] = "=" "\x1" "Coupon Vendor                        -0.34";// Item Head Gotten
	char buf3[] = "=" "\x1" "REMOVED       Coupon Vendor           0.34";// Item Gotten
	char buf4[] = "00FFFF=" "Cash                                 0.00";// Payment Type Gotten
	char buf5[] = "Please Come Again!                        ";// Thank Gotten

	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 34 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_THANK_GOTTEN );
}

void CEpsonCR0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "=" "\x1" "12/11/2003        10:32 AM          000190";// Time Gotten	
	char buf2[] = " NO SALE               ";	// No Sale Gotten
	char buf3[] = "Please Come Again!     ";	// Thank Gotten

	CReceiverStub receiver;

	CEpsonCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_NO_SALE_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONCR0S_STATE_THANK_GOTTEN );
}





