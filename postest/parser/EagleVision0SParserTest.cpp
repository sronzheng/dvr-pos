// CasioParserTest.cpp: implementation of the CEagleVision0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EagleVision0SParserTest.h"

#include "EagleVision0Sparser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CEagleVision0SParserTest );

CEagleVision0SParserTest::CEagleVision0SParserTest()
{

}

CEagleVision0SParserTest::~CEagleVision0SParserTest()
{
}

void CEagleVision0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = " 12/13/05  14:15  SYSTEM     551 SALE";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CEagleVision0SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Sale transaction
	// quantity:	2,			unit price:	2.00 dollar
    // name:		TEST SKU,	amount:		4.00 dollar  

	char buf1[] = "1                2   EA      2.00 EA";

	CPPUNIT_ASSERT( parser.setItemHead( buf1 ) );

	char buf2[] = "TEST SKU                         4.00";

	CPPUNIT_ASSERT( parser.setItemEnd( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "TEST SKU" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 4 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );
}

void CEagleVision0SParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// sale item.
	// 1. amount without dollar part.
	// 2. Sale transaction
	// quantity:	2,			unit price:	0.24 dollar
    // name:		TEST SKU,	amount:		0.48 dollar  

	char buf1[] = "1                2   EA      .24 EA";

	CPPUNIT_ASSERT( parser.setItemHead( buf1 ) );

	char buf2[] = "TEST SKU                         .48";

	CPPUNIT_ASSERT( parser.setItemEnd( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "TEST SKU" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 24 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 48 );
}

void CEagleVision0SParserTest::testSetItem3() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Refund transaction
	// quantity:	2,			unit price:	2.00 dollar
    // name:		TEST SKU,	amount:		4.02 dollar  

	char buf1[] = "1                2   EA      2.01 EA";

	CPPUNIT_ASSERT( parser.setItemHead( buf1 ) );

	char buf2[] = "TEST SKU                     (N 02      -4.02(N 01";

	CPPUNIT_ASSERT( parser.setItemEnd( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "TEST SKU" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 1 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -4 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -2 );
}

void CEagleVision0SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// subtotal. 
	// 1. amount with dollar part.
	// 2. Sale transaction
	// subtotal: 8.00 dollar, tax: 1.70 dorllar
	char buf[] = "SUB-TOTAL:       8.00    TAX:        1.70";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 70 );
}

void CEagleVision0SParserTest::testSetSubtotal2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// subtotal. 
	// 1. amount without dollar part.
	// 2. Sale transaction
	// subtotal: 0.98 dollar, tax: 0.08 dorllar
	char buf[] = "SUB-TOTAL:       .98    TAX:        0.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 98 );
	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 8 );
}

void CEagleVision0SParserTest::testSetSubtotal3() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// subtotal. 
	// 1. amount with dollar part.
	// 2. Sale transaction
	// subtotal: 8.01 dollar, tax: 1.70 dorllar
	char buf[] = "SUB-TOTAL:      -8.01    TAX:      -1.70";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == -8 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == -1 );
	CPPUNIT_ASSERT( receiver.m_tax.dollar == -1 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == -70 );
}

void CEagleVision0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// total. 
	// 1. amount with dollar part.
	// 2. Sale transaction
	// 8.70 dollar  
	char buf[] = "                       TOTAL:       8.70";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 70 );

}

void CEagleVision0SParserTest::testSetTotal2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// total. 
	// 1. amount with dollar part.
	// 2. Sale transaction
	// 0.70 dollar  
	char buf[] = "                       TOTAL:       0.70";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 70 );

}

void CEagleVision0SParserTest::testSetTotal3() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// total. 
	// 1. amount with dollar part.
	// 2. Refund transaction
	// 8.70 dollar  
	char buf[] = "                    TOTAL: (N 02     -8.70(N 01";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == -8 );
	CPPUNIT_ASSERT( receiver.m_total.cent == -70 );

}

void CEagleVision0SParserTest::testSetCashOfPayment1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// payment by cash. amount with dollar part.
    // cash: 10.00 dollar, change: 1.30 dollar  
	char buf[] = "CASH TEND:       10.00 CHANGE:       1.30";

	CPPUNIT_ASSERT( parser.setCashOfPayment( buf ) );

	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 10 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_change.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 30 );
}

void CEagleVision0SParserTest::testSetCashOfPayment2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// payment by cash. amount without dollar part.
    // cash: 0.50 dollar, change: 0.01 dollar  
	char buf[] = "CASH TEND:       .50 CHANGE:       .01";

	CPPUNIT_ASSERT( parser.setCashOfPayment( buf ) );

	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 50 );
	CPPUNIT_ASSERT( receiver.m_change.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 1 );
}

void CEagleVision0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// payment by check. amount with dolloar part
	// 8.70 dollar  
	char buf1[] = "CK#000123 ABA#        CK AMT:     8.70";
	char buf2[] = "                      BC AMT:     8.70";
	char buf3[] = "                   DEBIT/ATM:     8.70";
	char buf4[] = "GIFT CERTIFICATE         8.70";

	CPPUNIT_ASSERT( parser.setPaymentType( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CHECK );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 70 );

	CPPUNIT_ASSERT( parser.setPaymentType( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );

	CPPUNIT_ASSERT( parser.setPaymentType( buf3 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );

	CPPUNIT_ASSERT( parser.setPaymentType( buf4 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );
}

void CEagleVision0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// payment by check. amount without dolloar part
	// 0.70 dollar  
	char buf1[] = "CK#000123 ABA#        CK AMT:     .70";

	CPPUNIT_ASSERT( parser.setPaymentType( buf1 ) );

	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 70 );
}

void CEagleVision0SParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// payment by cash. amount with dollar part.
    // cash: 10.00 dollar, change: 1.30 dollar  
	char buf[] = "CASH TEND:       10.00 CHANGE:       1.30";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );

	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 10 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_change.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 30 );
}

void CEagleVision0SParserTest::testSetItemVoided1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// item voided, amount with dollar part.
	// 2.00 dollar
	char buf[] = "1              *ITEM VOIDED* [     2.01]";

	CPPUNIT_ASSERT( parser.setItemVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 1 );
}

void CEagleVision0SParserTest::testSetItemVoided2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// item voided, amount without dollar part.
	// 0.56 dollar
	char buf[] = "1              *ITEM VOIDED* [     .56]";

	CPPUNIT_ASSERT( parser.setItemVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 56 );
}

void CEagleVision0SParserTest::testSetRefund1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// refund. amount with dollar part
	// 2.19 dollar  
	char buf[] = "   REFUND:       2.19";

	CPPUNIT_ASSERT( parser.setRefund( buf ) );

	CPPUNIT_ASSERT( receiver.m_refund.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_refund.cent == 19 );

}

void CEagleVision0SParserTest::testSetRefund2() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// refund. amount without dollar part
	// 0.19 dollar  
	char buf[] = "   REFUND:       .19";

	CPPUNIT_ASSERT( parser.setRefund( buf ) );

	CPPUNIT_ASSERT( receiver.m_refund.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_refund.cent == 19 );

}

void CEagleVision0SParserTest::testsetTransactionVoided1() 
{
	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	// transaction voied. 
	char buf[] = "         **TRANSACTION VOIDED**";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}

void CEagleVision0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = " 1/19/06  9:50  SYSTEM     551 SALE		";// Date&Time Gotten
	char buf2[] = "1                 2   EA     4.01 EA     ";// Item Head Gotten
	char buf3[] = "TEST SKU                            8.02	";// Item Gotten
	char buf4[] = "1                 1   EA     2.00 EA     ";// Item 1st Line Gotten
	char buf5[] = "TEST SKU                             2.00";// Item Gotten
	char buf6[] = "1               *ITEM VOIDED* [     2.00]";// Item Gotten
	char buf7[] = "SUB-TOTAL:        8.00    TAX:        .70";// Subtotal Gotten
	char buf8[] = "                        TOTAL:       8.70";// Total Gotten
	char buf9[] = "CASH TEND:        9.00 CHANGE:        .30";// Payment Type Gotten
	char buf10[] = "GIFT CERTIFICATE        25.00           ";// Payment Type Gotten
	char buf11[] = "====>> JRNL# A07600           <<====	";// End Flag Gotten

	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 0 );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 9 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 0 );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 25 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 0 );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_END_FLAG_GOTTEN );
}

void CEagleVision0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = " 1/19/06  9:56  SYSTEM     551 REFUND	";// Date & Time Gotten
	char buf2[] = "1                 1   EA     1.00 EA     ";// Item Head Gotten
	char buf3[] = "TEST SKU            (N 02      -1.00(N 01";// Item Gotten
	char buf4[] = "SUB-TOTAL:      -1.01     TAX:       -.18";// Subtotal Gotten
	char buf5[] = "         TOTAL: .(N. 02      -1.19.(N. 01";// Total Gotten
	char buf6[] = "   REFUND:       1.19                    ";// Refund Gotten
	char buf7[] = "====>> JRNL# A07600             <<====   ";// End Flag Gotten

	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_REFUND_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_END_FLAG_GOTTEN );
}

void CEagleVision0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = " 1/19/06  9:56  SYSTEM     551 REFUND    ";// Date & Time Gotten
	char buf2[] = "1                 1   EA     2.00 EA     ";// Item Head Gotten
	char buf3[] = "TEST SKU                             2.00";// Item Gotten
	char buf4[] = "         **TRANSACTION VOIDED**          ";// Transaction Voided Gotten
	char buf5[] = "====>> JRNL# A07600             <<====   ";// End Flag Gotten

	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_END_FLAG_GOTTEN );
}

void CEagleVision0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = " 1/19/06  9:56  SYSTEM     551 REFUND    ";// Date & Time Gotten
	char buf2[] = "CARD TYPE:VISA                  EXPR:1210";// Card Type Gotten
	char buf3[] = "====>> JRNL# A07600             <<====   ";// End Flag Gotten

	CReceiverStub receiver;

	CEagleVision0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_CARD_TYPE_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EAGLEVISION0S_STATE_END_FLAG_GOTTEN );
}




