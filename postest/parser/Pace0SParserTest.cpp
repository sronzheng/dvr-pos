// Pace0SParserTest.cpp: implementation of the CPace0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pace0SParserTest.h"

#include "Pace0Sparser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CPace0SParserTest );

CPace0SParserTest::CPace0SParserTest()
{

}

CPace0SParserTest::~CPace0SParserTest()
{

}

void CPace0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 11:15
	char buf[] = "  CAS  12/13/05   11:15AM #00064323 31";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 11 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CPace0SParserTest::testSetTime2()
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 15:15
	char buf[] = "  CAS  12/13/05   03:15PM #00064323 31";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 15 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CPace0SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Sale transaction
	// quantity:	4,				unit price:	12.34 dollar
    // name: MISCELLANEOUS MERCH,	amount:		49.36 dollar  

	char buf1[] = "\xA" "13245      MISCELLANEOUS MERCH    49.36";
	char buf2[] = "             4 @     12.34";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItemMore( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "MISCELLANEOUS MERCH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 34 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 4 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 49 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 36 );
}

void CPace0SParserTest::testSetItem2() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Sale transaction
	// quantity:	1,				unit price:	12.34 dollar
    // name: MISCELLANEOUS MERCH,	amount:		12.34 dollar  

	char buf1[] = "\xA" "13245      MISCELLANEOUS MERCH    12.34";
	char buf2[] = "\xA" "13245      CUTTER1/8CONE TNGST    40.67";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "MISCELLANEOUS MERCH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 34 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 34 );
}

void CPace0SParserTest::testSetItem3() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Sale transaction
	// quantity:	4,				unit price:	12.34 dollar
    // name: MISCELLANEOUS MERCH,	amount:		49.36 dollar  

	char buf1[] = "\xA" "13245      MISCELLANEOUS MERCH    49.36";
	char buf2[] = "             4 @     12.34";
	char buf3[] = "\xA" "13245      CUTTER1/8CONE TNGST    40.67";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItemMore( buf2 ) );
	CPPUNIT_ASSERT( parser.setItem( buf3 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "MISCELLANEOUS MERCH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 34 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 4 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 49 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 36 );
}

void CPace0SParserTest::testSetItem4() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Sale transaction
	// quantity:	1,				unit price:	12.34 dollar
    // name: MISCELLANEOUS MERCH,	amount:		12.34 dollar  

	char buf1[] = "\xA" "13245      MISCELLANEOUS MERCH    -12.34";
	char buf2[] = "\xA" "13245      CUTTER1/8CONE TNGST    -40.67";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "MISCELLANEOUS MERCH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 34 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == -12 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -34 );
}

void CPace0SParserTest::testSetItem5() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// sale item. 
	// 1. amount with dollar part. 
	// 2. Sale transaction
	// quantity:	4,				unit price:	12.34 dollar
    // name: MISCELLANEOUS MERCH,	amount:		49.36 dollar  

	char buf1[] = "\xA" "13245      MISCELLANEOUS MERCH    12.34";
	char buf2[] = "SUBTOTAL               6.08";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setSubtotal( buf2 ) );
	
	CPPUNIT_ASSERT( parser.m_preItemGotten == false );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "MISCELLANEOUS MERCH" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 34 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 12 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 34 );
}

void CPace0SParserTest::testSetItem6()
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// sale item. 
    // transaction should be informed only if the first sale time is received.

	char buf1[] = "\xA" "13245      MISCELLANEOUS MERCH    12.34";
	char buf2[] = "\xA" "13245      CUTTER1/8CONE TNGST    40.67";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );
}

void CPace0SParserTest::testSetItem7()
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// next transation should be informed, if the sale item of next transaction received. 

	char buf1[] = "!#1  VOID TRANSACTION  ";
	char buf2[] = "   CAS  03/25/04   10:44AM #00064324 31";// Transaction voided Gotten

	CPPUNIT_ASSERT( parser.setTransactionVoided( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setTime( buf2 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == false );

}

void CPace0SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "SUBTOTAL               6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CPace0SParserTest::testSetSubtotal2() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// subtotal. 
	// subtotal: -6.08 dollar
	char buf[] = "SUBTOTAL               -6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == -6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == -8 );
}

void CPace0SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// subtotal. 
	// tax: 2.17 dorllar
	char buf[] = "TAX01                   2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CPace0SParserTest::testSetTax2() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// subtotal. 
	// tax: .17 dorllar
	char buf[] = "TAX01                   .17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CPace0SParserTest::testSetTax3() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// subtotal. 
	// tax: -2.17 dorllar
	char buf[] = "TAX01                   -2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == -2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == -17 );
}

void CPace0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "  TOTAL                 5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CPace0SParserTest::testSetTotal2() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// total. 
	// total:	-5.82 dollar  
	char buf[] = "  TOTAL                 -5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == -5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == -82 );
}

void CPace0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "CASH                   -6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == -6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == -35 );
}

void CPace0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// payment by gift certificate
	// amount:	5.82 dollar  
	char buf[] = "GIFT TEND               5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CPace0SParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// payment by check
	// amount:	5.82 dollar  
	char buf[] = "CHECK                 5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CHECK );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CPace0SParserTest::testSetPaymentType4() 
{
	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	// payment by bank card
	// amount:	5.82 dollar  
	char buf[] = "BANK CARD              5.82";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 82 );
}

void CPace0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "!#1     Welcome to ";				// Welcome Gotten
	char buf2[] = "!#2 Please Come Again";		
	char buf3[] = "!#1MISCELLANEOUS MERCHA";		
	char buf4[] = "!#2   1.00 @     3.33 ";	
	char buf5[] = "!#1AMOUNT DUE     30.17";		// Item Gotten
	char buf6[] = "!#2Thu   Mar 25    1904";		// Item Gotten
	char buf7[] = "!#1CASH           30.17";		
	char buf8[] = "!#2CHANGE          0.00";		
	char buf9[] = "             ";	
	char buf10[] = "33333      MISCELLANEOUS MERCH     3.33";// Item Gotten
	char buf11[] = "12223      SPACKL QT SYNKOLOID    22.45";
	char buf12[] = "             5 @       4.490";	// Item Gotten		
	char buf13[] = "SUBTOTAL               25.78";	// Subtotal Gotten
	char buf14[] = "TAX01                   2.17";	// Tax Gotten
	char buf15[] = "TOTAL                  27.95";	// Total Gotten
	char buf16[] = "CASH                    0.95";	// Payment Type Gotten
	char buf17[] = "GIFT TEND              27.00";	// Payment Type Gotten
	char buf18[] = "                    ";	
	char buf19[] = "   CAS  03/25/04   10:44AM #00064324 31";// Time Gotten

	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_HEAD_PASSED );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 33 );

	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SALE_ITEM_MORE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 22 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 45 );

	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf14 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf15 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf16 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf17 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );

	parser.parseLine( buf18 );
	parser.parseLine( buf19 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_DATE_TIME_GOTTEN );
}

void CPace0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "!#1     Welcome to ";				// Welcome Gotten
	char buf2[] = "!#2 Please Come Again";		
	char buf3[] = "!#1MISCELLANEOUS MERCHA";		
	char buf4[] = "!#2   1.00 @     3.33 ";	
	char buf5[] = "!#1AMOUNT DUE     30.17";		// Item Gotten
	char buf6[] = "!#2Thu   Mar 25    1904";		// Item Gotten
	char buf7[] = "!#1CASH           30.17";		
	char buf8[] = "!#2CHANGE          0.00";		
	char buf9[] = "             ";	
	char buf10[] = "12223      SPACKL QT SYNKOLOID    22.45";
	char buf11[] = "             5 @       4.490";	// Item Gotten		
	char buf12[] = "33333      MISCELLANEOUS MERCH     3.33";// Item Gotten
	char buf13[] = "SUBTOTAL               25.78";	// Subtotal Gotten
	char buf14[] = "TAX01                   2.17";	// Tax Gotten
	char buf15[] = "TOTAL                  27.95";	// Total Gotten
	char buf16[] = "CASH                    0.95";	// Payment Type Gotten
	char buf17[] = "GIFT TEND              27.00";	// Payment Type Gotten
	char buf18[] = "                    ";	
	char buf19[] = "   CAS  03/25/04   10:44AM #00064324 31";// Time Gotten

	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_HEAD_PASSED );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SALE_ITEM_MORE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 22 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 45 );

	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_SUBTOTAL_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 33 );

	parser.parseLine( buf14 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf15 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf16 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf17 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == GIFT_CERTIFICATE );

	parser.parseLine( buf18 );
	parser.parseLine( buf19 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_DATE_TIME_GOTTEN );
}

void CPace0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "!#1     Welcome to ";			// Welcome Gotten
	char buf2[] = "!#2 Please Come Again";		
	char buf3[] = "!#1MISCELLANEOUS MERCHA";		
	char buf4[] = "!#2   1.00 @     3.33 ";	
	char buf5[] = "!#1  RESET TRANSACTION";		// Transaction Reseted Gotten

	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_TRANSACTION_RESETED_GOTTEN );

}

void CPace0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "!#1     Welcome to      ";			// Welcome Gotten
	char buf2[] = "!#2 Please Come Again  ";		
	char buf3[] = "!#1MISCELLANEOUS MERCHA";		
	char buf4[] = "!#2   1.00 @     3.33  ";	
	char buf5[] = "!#1  VOID TRANSACTION  ";
	char buf6[] = "!#200064324            ";
	char buf7[] = "                       ";
	char buf8[] = "   CAS  03/25/04   10:44AM #00064324 31";// Transaction voided Gotten

	CReceiverStub receiver;

	CPace0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_WELCOME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_TRANSACTION_VOIDED_GOTTEN );

	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == PACE0S_STATE_DATE_TIME_GOTTEN );
}



