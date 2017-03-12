// NCR74500SParserTest.cpp: implementation of the CNCR74500SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCR74500SParserTest.h"

#include "NCR74500Sparser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CNCR74500SParserTest );

CNCR74500SParserTest::CNCR74500SParserTest()
{

}

CNCR74500SParserTest::~CNCR74500SParserTest()
{

}

void CNCR74500SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// data & time
	// 12-13-2005, 11:15
	char buf[] = "T#01 OP06 TRN2823 05/12/13 11:15 am";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 11 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CNCR74500SParserTest::testSetTime2()
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// data & time
	// 12-13-2005, 16:15
	char buf[] = "T#01 OP06 TRN2823 05/12/13 04:15 pm";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 16 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CNCR74500SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// sale item. 
    // name:		Newp Bx Kg,	amount:		8.99 dollar  

	char buf[] = "1  Newp Bx Kg                     8.99T";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Newp Bx Kg" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CNCR74500SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "SUBTOTAL             6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CNCR74500SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// subtotal. 
	// tax: 0.21 dorllar
	char buf[] = "SALES TAX ON 2.90          0.21 ";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 21 );
}

void CNCR74500SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "TOTAL DUE                          5.82 ";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CNCR74500SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "CASH                               6.35 ";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CNCR74500SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// payment by credit card
	// amount:	6.35 dollar  
	char buf[] = "SV Card                            6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CNCR74500SParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// payment by bank card
	// amount:	5.82 dollar  
	char buf[] = "DEBIT                    5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CNCR74500SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHANGE              7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CNCR74500SParserTest::testsetTransactionVoided1() 
{
	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	// transaction voied. 
	char buf[] = "<<< TRANSACTION ABORTED >>>             ";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}


void CNCR74500SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "            OH THANK HEAVEN         ";	// Time Gotten		
	char buf2[] = "T#01 OP06 TRN2825 03/03/19 01:23 pm ";		
	char buf3[] = "          THANKS FOR SHOPPING       ";	// Welcome Gotten
	char buf4[] = "                                    ";	
	char buf5[] = "1  MM Lemonade 20z Lse        1.19F ";	// Item Gotten
	char buf6[] = "1  MM FruitPnch20z            1.20F ";	// Item Gotten
	char buf7[] = "                                    ";		
	char buf8[] = "SUBTOTAL                      2.39  ";	// Subtotal Gotten
	char buf9[] = "TOTAL DUE                     2.39 ";	// Total Gotten
	char buf10[] = "CASH                         1.00 ";	// Payment Type Gotten
	char buf11[] = "DEBIT                        2.00 ";	// Payment Type Gotten
	char buf12[] = "CHANGE                       0.61 ";	// Change Gotten

	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 19 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 20 );

	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );

	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_CHANGE_GOTTEN );
}

void CNCR74500SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "            OH THANK HEAVEN         ";	// Time Gotten		
	char buf2[] = "T#01 OP06 TRN2825 03/03/19 01:23 pm ";		
	char buf3[] = "          THANKS FOR SHOPPING       ";	// Welcome Gotten
	char buf4[] = "                                    ";	
	char buf5[] = "1  Newp Bx Kg                 2.90T ";	// Item Gotten
	char buf6[] = "                                    ";		
	char buf7[] = "SUBTOTAL                       2.90 ";	// Subtotal Gotten
	char buf8[] = "SALES TAX ON 2.90              0.21 ";	// Tax Gotten
	char buf9[] = "TOTAL DUE                      3.11 ";	// Total Gotten
	char buf10[] = "CASH                          3.11 ";	// Payment Type Gotten

	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_TAX_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
}

void CNCR74500SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "            OH THANK HEAVEN         ";	// Time Gotten		
	char buf2[] = "T#01 OP06 TRN2825 03/03/19 01:23 pm ";		
	char buf3[] = "          THANKS FOR SHOPPING       ";	// Welcome Gotten
	char buf4[] = "                                    ";	
	char buf5[] = "1  Newp Bx Kg                 2.90T ";	// Item Gotten
	char buf6[] = " ---------------------------------- ";		
	char buf7[] = "<<< TRANSACTION ABORTED >>>         ";	// Transaction Gotten

	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_TRANSACTION_VOIDED_GOTTEN );
}

void CNCR74500SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "            OH THANK HEAVEN         ";	// Time Gotten		
	char buf2[] = "T#01 OP06 TRN2825 03/03/19 01:23 pm ";		
	char buf3[] = "          THANKS FOR SHOPPING       ";	// Welcome Gotten
	char buf4[] = "                                    ";	
	char buf5[] = "     NO SALE                        ";	// No Sale Gotten

	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_NO_SALE_GOTTEN );
}

void CNCR74500SParserTest::testActualParse5()
{
	// state transform.

	char buf1[] = "            OH THANK HEAVEN         ";	// Time Gotten		
	char buf2[] = "T#01 OP06 TRN2825 03/03/19 01:23 pm ";		
	char buf3[] = "          THANKS FOR SHOPPING       ";	// Welcome Gotten
	char buf4[] = "                                    ";	
	char buf5[] = "   SAFE DROP                        ";	// No Sale Gotten

	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_SAFE_DROP_GOTTEN );
}

void CNCR74500SParserTest::testActualParse6()
{
	// state transform.

	char buf1[] = "            OH THANK HEAVEN         ";	// Time Gotten		
	char buf2[] = "T#01 OP06 TRN2825 03/03/19 01:23 pm ";		
	char buf3[] = "          THANKS FOR SHOPPING       ";	// Welcome Gotten
	char buf4[] = "                                    ";	
	char buf5[] = "1  Newp Bx Kg                 2.90T ";	// Item Gotten
	char buf6[] = "                                    ";		
	char buf7[] = "SUBTOTAL                       2.90 ";	// Subtotal Gotten
	char buf8[] = "SALES TAX ON 2.90              0.21 ";	// Tax Gotten
	char buf9[] = "TOTAL DUE                      3.11 ";	// Total Gotten
	char buf10[] = "CASH                          3.11 ";	// Payment Type Gotten
	char buf11[] ="            OH THANK HEAVEN         ";	// Time Gotten		
	char buf12[] ="T#01 OP06 TRN2825 03/03/19 01:24 pm ";		

	CReceiverStub receiver;

	CNCR74500SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == NCR74500S_STATE_DATE_TIME_GOTTEN );
}



