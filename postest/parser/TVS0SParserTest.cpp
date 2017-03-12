// TVS0SParserTest.cpp: implementation of the CTVS0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TVS0SParserTest.h"

#include "TVS0Sparser.h"

#include "receiverstub.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CTVS0SParserTest );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTVS0SParserTest::CTVS0SParserTest()
{

}

CTVS0SParserTest::~CTVS0SParserTest()
{

}

void CTVS0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = "12/13/05 14:15         1TI";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CTVS0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// sale item. 
    // name:		T  BLACK V,	amount:		8.99 dollar  

	char buf[] = "T  BLACK V 04246   899 1";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "T  BLACK V" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CTVS0SParserTest::testSetItem2()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// sale item. 
    // name:		T  BLACK V,	amount:		-8.99 dollar  

	char buf[] = "T  BLACK V 04246  -899 1";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "T  BLACK V" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -99 );
}

void CTVS0SParserTest::testSetItem3()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// sale item. 
    // name:		PRO,	amount:		-0.45 dollar  

	char buf[] = "PRO               -045 1";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "PRO" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -45 );
}

void CTVS0SParserTest::testSetItem4()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// sale item. 
    // name:		PRO,	amount:		-0.45 dollar  

	char buf[] = "PRO               -145 1";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "PRO" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -45 );
}


void CTVS0SParserTest::testSetItem5()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// item between sale items ignored. 
	char buf[] = "                  2000 1YY";

	CPPUNIT_ASSERT( parser.setItem( buf ) == false );
}

void CTVS0SParserTest::testSetItem6()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// item between sale items ignored. 
	char buf[] = "";

	CPPUNIT_ASSERT( parser.setItem( buf ) == false );
}

void CTVS0SParserTest::testSetItem7()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// \r\n should be ignored as space character.
 
	char buf[] = "T  BLACK V\r\n 04246\r\n  -899 1";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "T  BLACK V" )  == 0 );
}

void CTVS0SParserTest::testSetItem8()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// transation should be informed, only if the first sale item received. 

	char buf1[] = "T  BLACK V 04246   899 1";
	char buf2[] = "T  BLACK V 04246   899 1";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );
}

void CTVS0SParserTest::testSetItem9()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// next transation should be informed, if the sale item of next transaction received. 

	char buf1[] = "T  COORS L 03258   536 1";	
	char buf2[] = "TAX                030 1TX";	
	char buf3[] = "TOTAL              430 1TT";	

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setTax( buf2 ) );
	CPPUNIT_ASSERT( parser.setTotal( buf3 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == false );

}

void CTVS0SParserTest::testSetTax1()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// tax. 
	// tax: 0.45 dorllar
	char buf[] = "TAX                045 1TX";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 45 );
}

void CTVS0SParserTest::testSetTotal1()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// total. 
	// amount : 31.75 dollar  
	char buf[] = "TOTAL             3175 1TT";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 31 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 75 );
}

void CTVS0SParserTest::testSetPaymentType1()
{
	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	// payment by check. amount with dolloar part
	// 8.70 dollar  
	char buf1[] = "CASH            2100   1AM";
	char buf2[] = "CREDIT          3175   1CG";

	CPPUNIT_ASSERT( parser.setPaymentType( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 21 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 0 );

	CPPUNIT_ASSERT( parser.setPaymentType( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );
}

void CTVS0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "T  COORS L 03258   536 1";	// Item Gotten
	char buf2[] = "T   BEER DI 00225 -136 1";	// Item Gotten
	char buf3[] = "TAX                030 1TX";	// Tax Gotten
	char buf4[] = "TOTAL              430 1TT";	// Total Gotten
	char buf5[] = "CASH              1000 1AM";	// Payment Type Gotten
	char buf6[] = "010                    1CL";
	char buf7[] = "23520                  1TK";
	char buf8[] = "01/22/04 14:06         1TI";	// Date & Time Gotten

	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 36 );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -36 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_PAYMENT_TYPE_GOTTEN );

	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_DATE_TIME_GOTTEN );
}

void CTVS0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "SAFE LOAN         6000 1";	// Safe Loan Gotten
	char buf2[] = "010                    1CL";	
	char buf3[] = "23544                  1TK";	
	char buf4[] = "01/22/04 14:32         1TI";	// Date & Time Gotten

	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_SAFE_LOAN_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_DATE_TIME_GOTTEN );
}

void CTVS0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "NO SALE                1NS";	// No Sale Gotten
	char buf2[] = "010                    1CL";	
	char buf3[] = "23544                  1TK";	
	char buf4[] = "01/22/04 14:32         1TI";	// Date & Time Gotten

	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_NO_SALE_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_DATE_TIME_GOTTEN );
}

void CTVS0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "PAY OUT            200 1PO";	// Pay out Gotten
	char buf2[] = "010                    1CL";	
	char buf3[] = "23544                  1TK";	
	char buf4[] = "01/22/04 14:32         1TI";	// Date & Time Gotten

	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_PAY_OUT_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_DATE_TIME_GOTTEN );
}

void CTVS0SParserTest::testParse1()
{
	// handle multiple lines in one time.

	char buf[] = "T   BEER DI 00225 -136 1\xC8"
				 "TAX                030 1TX\xC8";

	CReceiverStub receiver;

	CTVS0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_BEGINNING );

	CPPUNIT_ASSERT( parser.parse( buf ) == buf );
	CPPUNIT_ASSERT( parser.m_state == TVS0S_STATE_TAX_GOTTEN );
}



