// EpsonIStore0SParserTest.cpp: implementation of the CEpsonIStore0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EpsonIStore0SParserTest.h"

#include "EpsonIStore0SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CEpsonIStore0SParserTest );

CEpsonIStore0SParserTest::CEpsonIStore0SParserTest()
{

}

CEpsonIStore0SParserTest::~CEpsonIStore0SParserTest()
{

}

void CEpsonIStore0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = "Date  : 12/13/05        Time : 14:15";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CEpsonIStore0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// sale item. 
    // name:	LEAKSTOP 250ML NEWDEN,	amount:		8.99 dollar  

	char buf[] = "LEAKSTOP 250ML NEWDEN              8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) == EPSONISTORE0S_WHOLE_ITEM );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "LEAKSTOP 250ML NEWDEN" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 99 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CEpsonIStore0SParserTest::testSetItem2()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// sale item. 
    // name:	LEAKSTOP 250ML NEWDEN,	amount:		(invisible)

	char buf[] = "LEAKSTOP 250ML NEWDEN              *.**";

	CPPUNIT_ASSERT( parser.setItem( buf ) == EPSONISTORE0S_WHOLE_ITEM );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "LEAKSTOP 250ML NEWDEN" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );
}

void CEpsonIStore0SParserTest::testSetItem3()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// sale item. 
    // name:   PRIMUS SILENT BURNER 2.170,	amount:		59.94 dollar  
    // quantity:				6,			unit price:	9.99 dollar  

	char buf1[] = "PRIMUS SILENT BURNER 2.170";
	char buf2[] = "62114         *  6    @    9.99=   59.94";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) == EPSONISTORE0S_HEAD_ITEM );
	CPPUNIT_ASSERT( strcmp( parser.m_itemName, "PRIMUS SILENT BURNER 2.170" )  == 0 );

	CPPUNIT_ASSERT( parser.setItemMore( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_itemName.compare( "PRIMUS SILENT BURNER 2.170" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 9 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 99 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 6 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 59 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 94 );
}

void CEpsonIStore0SParserTest::testSetItem4()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// sale item. 
    // name:   PUMP PARAFFIN 20LT,	amount:		-59.94 dollar  
    // quantity:				6,	unit price:	-9.99 dollar  

	char buf1[] = "PUMP PARAFFIN 20LT";
	char buf2[] = "62114         *  6    @    -9.99=   -59.94";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) == EPSONISTORE0S_HEAD_ITEM );
	CPPUNIT_ASSERT( strcmp( parser.m_itemName, "PUMP PARAFFIN 20LT" )  == 0 );

	CPPUNIT_ASSERT( parser.setItemMore( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_itemName.compare( "PUMP PARAFFIN 20LT" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == -9 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == -99 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 6 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -59 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -94 );
}

void CEpsonIStore0SParserTest::testSetItem5()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// sale item. 
    // name:  LEG WINE SET 3PCE T/BOX,	amount:		(invisible)
    // quantity:     	(invisible)	,	unit price:	203.00 dollar

	char buf1[] = "LEG WINE SET 3PCE T/BOX";
	char buf2[] = "065255057674  ****    @  203.00=*****.**";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) == EPSONISTORE0S_HEAD_ITEM );
	CPPUNIT_ASSERT( parser.setItemMore( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 203 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );
}

void CEpsonIStore0SParserTest::testSetItem6()
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// sale item. 
    // name:	*ROUNDING OFF DISCOUNT,	amount:		-8.99 dollar  

	char buf[] = "*ROUNDING OFF DISCOUNT             -8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) == EPSONISTORE0S_WHOLE_ITEM );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "*ROUNDING OFF DISCOUNT" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == -8 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == -99 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -99 );
}

void CEpsonIStore0SParserTest::testSetItemVoided1() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// item voided, 
	// 2.01 dollar
	char buf[] = "*REVERSE ROUNDING OFF DISCOUNT    -2.01";

	CPPUNIT_ASSERT( parser.setItemVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == 1 );
}

void CEpsonIStore0SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "TOTAL                     6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CEpsonIStore0SParserTest::testSetSubtotal2() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// subtotal. 
	// subtotal: (invisible)
	char buf[] = "TOTAL                           *****.**";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 0 );
}

void CEpsonIStore0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "TOTAL AMOUNT                      5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CEpsonIStore0SParserTest::testSetTotal2() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// total. 
	// total:	(invisible)
	char buf[] = "TOTAL AMOUNT                    *****.**";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 0 );
}

void CEpsonIStore0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "Cash                              6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CEpsonIStore0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHANGE                7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CEpsonIStore0SParserTest::testSetChange2() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// change
    // amount: (invisbile)
	char buf[] = "CHANGE               *****.**";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 0 );
}

void CEpsonIStore0SParserTest::testsetTransactionVoided1() 
{
	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	// transaction voied. 
	char buf[] = "TRANSACTION VOID !!!             -263.89";

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf ) );

	CPPUNIT_ASSERT( receiver.m_transactionVoided == true );
}

void CEpsonIStore0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "Date  : 24/09/04            Time : 21:49";	// Time Gotten		
	char buf2[] = "PRIMUS SILENT BURNER 2.170         17.35";	// Item Gotten
	char buf3[] = "HIPPOLITO ROARER BURNER 1.25       21.37";	// Item Gotten
	char buf4[] = "PUMP PARAFFIN 20LT";							// Item Start Gotten
	char buf5[] = "62114         *  5    @    9.99=   49.95";	// Item End Gotten
	char buf6[] = "TOTAL                              88.67";	// Subtotal Gotten
	char buf7[] = "TOTAL AMOUNT                       88.67";	// Total Gotten
	char buf8[] = "Cash                              100.00";	// Payment Type Gotten
	char buf9[] = "CHANGE                             11.33";	// Change Gotten
	char buf10[] = "******** PLEASE COME AGAIN ************";	// Thank Gotten
	char buf11[] = "   *******COMPETITION ENTRY SLIP*******";	// Complete Start Gotten
	char buf12[] = "=======================================";	// Complete End Gotten

	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 17 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 35 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 21 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 37 );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 9 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 99 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 49 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 95 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_THANK_GOTTEN );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_COMPLETION_START_GOTTEN );

	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_COMPLETION_END_GOTTEN );
}

void CEpsonIStore0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "Date  : 24/09/04            Time : 21:49";	// Time Gotten		
	char buf2[] = "PRIMUS SILENT BURNER 2.170         17.35";	// Item Gotten
	char buf3[] = "TOTAL                              17.35";	// Subtotal Gotten
	char buf4[] = "TOTAL AMOUNT                       17.35";	// Total Gotten
	char buf5[] = "********* PLEASE COME AGAIN ************";	// Thank Gotten
	char buf6[] = "Cash                               20.00";	// Payment Type Gotten
	char buf7[] = "CHANGE                              2.65";	// Change Gotten
	char buf8[] = "******** PLEASE COME AGAIN ************";	// Thank Gotten
	char buf9[] = "Date  : 24/09/04            Time : 21:52";	// Time Gotten		

	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_THANK_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_THANK_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );
}

void CEpsonIStore0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "Date  : 24/09/04            Time : 21:49";	// Time Gotten		
	char buf2[] = "PRIMUS SILENT BURNER 2.170         17.35";	// Item Gotten
	char buf3[] = "TRANSACTION VOID !!!             -263.89";	// Transaction Voided Gotten

	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_TRANSACTION_VOIDED_GOTTEN );
}

void CEpsonIStore0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "Date  : 24/09/04            Time : 21:49";	// Time Gotten		
	char buf2[] = "TRANSACTION VOID !!!             -263.89";	// Transaction Voided Gotten

	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_TRANSACTION_VOIDED_GOTTEN );
}

void CEpsonIStore0SParserTest::testActualParse5()
{
	// state transform.

	char buf1[] = "Date  : 24/09/04            Time : 21:49";	// Time Gotten		
	char buf2[] = "Date  : 24/09/04            Time : 21:52";	// Time Gotten		

	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );
}

void CEpsonIStore0SParserTest::testActualParse6()
{
	// state transform.

	char buf1[] = "Date  : 24/09/04            Time : 21:49";	// Time Gotten		
	char buf2[] = "TOTAL                              17.35";	// Subtotal Gotten
	char buf3[] = "TOTAL AMOUNT                       17.35";	// Total Gotten
	char buf4[] = "********* PLEASE COME AGAIN ************";	// Thank Gotten

	CReceiverStub receiver;

	CEpsonIStore0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == EPSONISTORE0S_STATE_THANK_GOTTEN );

}
