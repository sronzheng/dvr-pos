// SharpParserTest.cpp: implementation of the CSharpParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SharpParserTest.h"

#include "Sharp0Sparser.h"
#include "receiverstub.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CSharpParserTest );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharpParserTest::CSharpParserTest()
{

}

CSharpParserTest::~CSharpParserTest()
{

}

void CSharpParserTest::testSetDate1()
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// data & time
	// 11-22-2004, 02:03
	char buf1[] = "c4\xA" "11/22/04\x1D!         000004";
	char buf2[] = "#0021      2:03AM SERV.0040004";

	CPPUNIT_ASSERT( parser.setDate( buf1 ) );
	CPPUNIT_ASSERT( parser.setTime( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2004 );
	CPPUNIT_ASSERT( receiver.m_month == 11 );
	CPPUNIT_ASSERT( receiver.m_day == 22 );
	CPPUNIT_ASSERT( receiver.m_hour == 2 );
	CPPUNIT_ASSERT( receiver.m_minute == 3 );
}

void CSharpParserTest::testSetDate2()
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// data & time .afternoon time
	// 04-22-2004, 23:03
	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";
	char buf2[] = "#0021      2:03PM SERV.0040004";

	CPPUNIT_ASSERT( parser.setDate( buf1 ) );
	CPPUNIT_ASSERT( parser.setTime( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_year == 2004 );
	CPPUNIT_ASSERT( receiver.m_month == 4 );
	CPPUNIT_ASSERT( receiver.m_day == 22 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 3 );
}

void CSharpParserTest::testSetDate3()
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// data & time. hour is of two digits.
	// 04-22-2004, 23:03
	char buf1[] = "c4\xA" "c4 04/22/04\x1D!         000004";
	char buf2[] = "#0021      11:03PM SERV.0040004";

	CPPUNIT_ASSERT( parser.setDate( buf1 ) );
	CPPUNIT_ASSERT( parser.setTime( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_year == 2004 );
	CPPUNIT_ASSERT( receiver.m_month == 4 );
	CPPUNIT_ASSERT( receiver.m_day == 22 );
	CPPUNIT_ASSERT( receiver.m_hour == 23 );
	CPPUNIT_ASSERT( receiver.m_minute == 3 );
}

void CSharpParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// sale item. 
    // name:		6 DONUTS,	amount:		3.59 dollar  

	char buf[] = "! 6 DONUTS               $3.59";
	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "6 DONUTS" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 59 );
}

void CSharpParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// discount. 
    // name:		FREE DON W-COF,	amount:		0.59 dollar  

	char buf[] = "! FREE DON W-COF         -0.59";
	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "FREE DON W-COF" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -59 );
}

void CSharpParserTest::testSetItem3() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// item voided.
	// 0.70 dollar
	char buf[] = "! 1 DONUT                 V-0.70";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == -70 );
}

void CSharpParserTest::testSetItem4() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// item voided.
	// 0.70 dollar
	char buf[] = "! 6 DONUTS                R-0.70";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_amountVoided.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_amountVoided.cent == -70 );
}

void CSharpParserTest::testSetItem5() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// item prefix. ignore it.
	char buf[] = "!                     6/ $3.59";

	CPPUNIT_ASSERT( parser.setItem( buf ) == false );
}

void CSharpParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "MDSE ST                $6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CSharpParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// subtotal. 
	// tax: 0.13 dorllar
	char buf[] = "TAX1                   $0.13";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 13 );
}

void CSharpParserTest::testSetTax2() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// subtotal. 
	// tax: -0.08 dorllar
	char buf[] = "TAX1                     -0.08";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == -8 );
}

void CSharpParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "***TOTAL            !$5.82!";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );

}

void CSharpParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "C! ASH             !$6.35!";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CSharpParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// payment by gift certificate
	// amount:	5.82 dollar  
	char buf[] = "GIFT RED               $5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CSharpParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// payment by charge?
	// amount:	2.85 dollar  
	char buf[] = "CHARGE3               !$2.85!";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
}

void CSharpParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHANGE                   $7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CSharpParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";		// Date Gotten
	char buf2[] = "! #0022    2:05AM SERV.0040004";		// Time Gotten
	char buf3[] = "! 2OZ FLAV CC            $1.10";		// Item Gotten
	char buf4[] = "!                     6/ $3.59";	
	char buf5[] = "! 6 BAGELS               $3.59";		// Item Gotten
	char buf6[] = "! FREE DON W-COF         -0.59";		// Item Gotten
	char buf7[] = "! MDSE ST                $4.10";		// Subtotal Gotten
	char buf8[] = "! TAX1                   $0.13";		// Tax Gotten
	char buf9[] = "! ***TOTAL            !$4.23!";	// Total Gotten
	char buf10[] = "! GIFT RED               $4.00";	// Payment Type Gotten
	char buf11[] = "!C! ASH                 !$0.50!";	// Payment Type Gotten
	char buf12[] = "! CHANGE                 $0.27";	// Change Gotten

	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_DATE_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 10 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 10 );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 59 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_CHANGE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_change.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 27 );
}

void CSharpParserTest::testActualParse2()
{
	// state transform.
	// important: Tax Gotten -> Payment Type Gotten
	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";		// Date Gotten
	char buf2[] = "! #0021      2:03AM SERV.0040004";	// Time Gotten
	char buf3[] = "! COMBO #1               $2.81";		// Item Gotten
	char buf4[] = "!   MD HOT COFFEE";	
	char buf5[] = "!   2 DONUTS";		
	char buf6[] = "! MDSE ST                $2.81";		// Subtotal Gotten
	char buf7[] = "! TAX1                   $0.27";		// Tax Gotten
	char buf8[] = "!C! ASH                 !$3.08!";	// Payment Type Gotten

	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_DATE_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
}

void CSharpParserTest::testActualParse3()
{
	// state transform.
	// important: Item Gotten -> Payment Type Gotten
	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";		// Date Gotten
	char buf2[] = "! #0021      2:03AM SERV.0040004";	// Time Gotten
	char buf3[] = "! 1 DONUT                  $0.70";	// Item Gotten
	char buf4[] = "! 1 DONUT                 V-0.70";	// Item Gotten
	char buf5[] = "!C! ASH                   !$0.00!";	// Payment Type Gotten

	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_DATE_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
}

void CSharpParserTest::testActualParse4()
{
	// state transform.
	// important: Time Gotten -> Payment Type Gotten

	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";		// Date Gotten
	char buf2[] = "! #0021      2:03AM SERV.0040004";	// Time Gotten
	char buf3[] = "!   SM DECAF TEA";					
	char buf4[] = "!   GENERIC COFFEE";					
	char buf5[] = "!C! ASH                 !$0.00! ";	// Payment Type Gotten

	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_DATE_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
}

void CSharpParserTest::testActualParse5()
{
	// state transform.
	// important: Item Gotten -> Total Gotten

	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";		// Date Gotten
	char buf2[] = "! #0021      2:03AM SERV.0040004";	// Time Gotten
	char buf3[] = "! BAGEL W/FLAVORED         $1.79";	// Item Gotten				
	char buf4[] = "! ***TOTAL                !$1.79!";	// Total Gotten				
	char buf5[] = "!C! ASH                   !$2.00!";	// Payment Type Gotten
	char buf6[] = "! CHANGE                   $0.21";	// Change Gotten

	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_DATE_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_CHANGE_GOTTEN );
}

void CSharpParserTest::testActualParse6()
{
	// state transform.
	// important: Time Gotten -> No Sale Gotten

	char buf1[] = "c4\xA" "04/22/04\x1D!         000004";		// Date Gotten
	char buf2[] = "! #0021      2:03AM SERV.0040004";	// Time Gotten
	char buf3[] = "! NO SALE";							// No Sale Gotten				

	CReceiverStub receiver;

	CSharp0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_DATE_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_TIME_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == SHARP0S_STATE_NO_SALE_GOTTEN );
}



