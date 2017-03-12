// RubyCR0SParserTest.cpp: implementation of the CRubyCR0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RubyCR0SParserTest.h"

#include "RubyCR0Sparser.h"
#include "receiverstub.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CRubyCR0SParserTest );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRubyCR0SParserTest::CRubyCR0SParserTest()
{

}

CRubyCR0SParserTest::~CRubyCR0SParserTest()
{

}

void CRubyCR0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// data & time
	// 02-04-2005, 09:16
	char buf[] = "  02/04/05  09:16:47           ST# GVG02";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 2 );
	CPPUNIT_ASSERT( receiver.m_day == 4 );
	CPPUNIT_ASSERT( receiver.m_hour == 9 );
	CPPUNIT_ASSERT( receiver.m_minute == 16 );
}

void CRubyCR0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// sale item. 
    // name:		T  BUD 12P,	amount:		8.99 dollar  

	char buf[] = "\x1T  BUD 12P 00333        1           8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "T  BUD 12P" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CRubyCR0SParserTest::testSetItem2()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// sale item. discount.
    // name:		Pro,	amount:		-0.15 dollar  

	char buf[] = "\x1         Pro                       -0.15";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Pro" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == -15 );
}

void CRubyCR0SParserTest::testSetItem3()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// sale item. discount.
    // name:		Pro,	amount:		-0.15 dollar  

	char buf1[] = "\x1         Pro                       -0.15";
	char buf2[] = "\x1         Pro                       -0.15";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Pro" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 1 );
}

void CRubyCR0SParserTest::testSetItem4()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// sale item. 
    // name:		T  BUD 12P,	amount:		8.99 dollar  

	char buf1[] = "\x1T  BUD 12P 00333        1           8.99";
	char buf2[] = "\x1T  BUD 12P 00333        1           8.99";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "T  BUD 12P" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 1 );
}

void CRubyCR0SParserTest::testSetItem5()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// sale item. 
    // name:		T  BUD 12P,	amount:		8.99 dollar  
    // name:		Pro,	amount:		-0.15 dollar  

	char buf1[] = "\x1T  BUD 12P 00333        1           8.99";
	char buf2[] = "\x1         Pro                       -0.15";
	char buf3[] = "\x1T  BUD 12P 00333        1           8.99";
	char buf4[] = "\x1         Pro                       -0.15";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( parser.setItem( buf3 ) );
	CPPUNIT_ASSERT( parser.setItem( buf4 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "Pro" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 2 );
}

void CRubyCR0SParserTest::testSetItem6()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

    // transaction should be informed, only if the first sale item is received.

	char buf1[] = "\x1T  BUD 12P 00333        1           8.99";
	char buf2[] = "\x1T  BUD 12P 00333        1           8.99";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );
}

void CRubyCR0SParserTest::testSetItem7()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// next transation should be informed, if the sale item of next transaction received. 

	char buf1[] = "\x1T  BUD 12P 00333        1        8.99";
	char buf2[] = "                  Sub Total         8.99";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == true );

	CPPUNIT_ASSERT( parser.setSubtotal( buf2 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == false );
}

void CRubyCR0SParserTest::testSetSubtotal1()
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// subtotal. 
    // amount:		8.99 dollar  
	char buf[] = "                  Sub Total         8.99";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 99 );
}

void CRubyCR0SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// tax. 
	// tax: 0.26 dorllar
	char buf[] = "      Tax         0.26";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 26 );
}

void CRubyCR0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "!!                   TOTAL         5.82!";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CRubyCR0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "              Cash  $     6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CRubyCR0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// payment by credit card
	// amount:	6.35 dollar  
	char buf[] = "             Credit  $     6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CRubyCR0SParserTest::testSetPaymentType3() 
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf1[] = "              Cash  $     6.35";
	char buf2[] = "              Cash  $     6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf1 ) );
	CPPUNIT_ASSERT( parser.setPaymentType( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
	CPPUNIT_ASSERT( receiver.m_CountOfPayment == 1 );
}


void CRubyCR0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHANGE                   $7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CRubyCR0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "***** ERROR CORRECTION *******";			// Erorr Head Gotten
	char buf2[] = "\x1T T MARLBOR 03458  1    3.65";		
	char buf3[] = "\x1  Pro                  -0.15";		
	char buf4[] = "******************************";			// Erorr Tail Gotten
	char buf5[] = "\x1T T MARLBOR 03458  1    3.65";		// Item Gotten
	char buf6[] = "\x1T T MARLBOR 03458  1    3.65";		
	char buf7[] = "\x1  Pro                  -0.15";		// Item Gotten
	char buf8[] = "\x1T T MARLBOR 03458  1    3.65";		// Item Gotten
	char buf9[] = "\x1  Pro                  -0.15";		
	char buf10[] = "                     ----------";		
	char buf11[] = "         Sub Total         7.00";		// Subtotal Gotten
	char buf12[] = "\x1  Pro                  -0.15";		
	char buf13[] = "\x1  T MARLBOR 03462  1    3.65";	
	char buf14[] = "\x1 Pro                  -0.15";	
	char buf15[] = "              Tax         0.53";		// Tax Gotten
	char buf16[] = "              Tax         0.53";
	char buf17[] = "!           TOTAL        7.53!";		// Total Gotten
	char buf18[] = "             Cash  $      5.00";		// Payment Type Gotten
	char buf19[] = "!           TOTAL        7.53!";	
	char buf20[] = "             Cash  $      5.00";	
	char buf21[] = "             Cash  $      5.00";		// Payment Type Gotten
	char buf22[] = "           Change  $     -2.47";		// Change Gotten
	char buf23[] = "                              ";	
	char buf24[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";	
	char buf25[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten
	char buf26[] = "                              ";	
	char buf27[] = "       Thanks for shopping    ";		// Thank Gotten
	char buf28[] = "       Green Valley Grocery #2";	
	char buf29[] = "                              ";	
	char buf30[] = "1D32456092-001                ";	
	char buf31[] = "   Descr.     qty       amount";	
	char buf32[] = "              Cash  $      5.0";	
	char buf33[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";
	char buf34[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten

	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_ERROR_HEAD_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 0 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_ERROR_TAIL_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 1 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 1 );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 2 );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 3 );

	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfSetItem == 3 );

	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	parser.parseLine( buf13 );
	parser.parseLine( buf14 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf15 );
	parser.parseLine( buf16 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf17 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_TOTAL_GOTTEN );

	CPPUNIT_ASSERT( receiver.m_CountOfPayment == 0 );

	parser.parseLine( buf18 );
	parser.parseLine( buf19 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_CountOfPayment == 1 );

	parser.parseLine( buf20 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfPayment == 1 );

	parser.parseLine( buf21 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_CountOfPayment == 2 );

	parser.parseLine( buf22 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_CHANGE_GOTTEN );

	CPPUNIT_ASSERT( receiver.m_CountOfDateTime == 0 );

	parser.parseLine( buf23 );
	parser.parseLine( buf24 );
	parser.parseLine( buf25 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_CountOfDateTime == 1 );

	parser.parseLine( buf26 );
	parser.parseLine( buf27 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_THANK_GOTTEN );

	parser.parseLine( buf28 );
	parser.parseLine( buf29 );
	parser.parseLine( buf30);
	parser.parseLine( buf31 );
	parser.parseLine( buf32 );
	parser.parseLine( buf33 );
	parser.parseLine( buf34 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN );
}

void CRubyCR0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "\x1T T MARLBOR 03458  1    3.65";		// Item Gotten
	char buf2[] = "\x1T T MARLBOR 03458  1    3.65";		
	char buf3[] = "****** ERROR CORRECTION *******";		// Erorr Head Gotten
	char buf4[] = "\x1T T MARLBOR 03458  1    3.65";		
	char buf5[] = "\x1  Pro                  -0.15";		
	char buf6[] = "*******************************";		// Erorr Tail Gotten
	char buf7[] = "                     ----------";		
	char buf8[] = "         Sub Total        3.65";			// Subtotal Gotten
	char buf9[] = "              Tax         0.53";			// Tax Gotten
	char buf10[] = "              Tax         0.53";
	char buf11[] = "!           TOTAL        4.18!";		// Total Gotten
	char buf12[] = "!           TOTAL        4.18!";	
	char buf13[] = "            Credit  $     4.18";		// Payment Type Gotten
	char buf14[] = "                              ";	
	char buf15[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";	
	char buf16[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten
	char buf17[] = "                              ";	
	char buf18[] = "       Thanks for shopping    ";		// Thank Gotten
	char buf19[] = "       Green Valley Grocery #2";	
	char buf20[] = "                              ";	
	char buf21[] = "1D32456092-001                ";	
	char buf22[] = "   Descr.     qty       amount";	
	char buf23[] = "              Cash  $      5.0";	
	char buf24[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";
	char buf25[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten
	char buf26[] = "                              ";	
	char buf27[] = "\x1T T MARLBOR 03458  1    3.65";		// Item Gotten
	char buf28[] = "\x1T T MARLBOR 03458  1    3.65";		

	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_ERROR_HEAD_GOTTEN );

	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_ERROR_TAIL_GOTTEN );

	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CREDIT );

	parser.parseLine( buf14 );
	parser.parseLine( buf15 );
	parser.parseLine( buf16 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN );

	parser.parseLine( buf17 );
	parser.parseLine( buf18 );
	parser.parseLine( buf19 );
	parser.parseLine( buf20 );
	parser.parseLine( buf21 );
	parser.parseLine( buf22 );
	parser.parseLine( buf23 );
	parser.parseLine( buf24 );
	parser.parseLine( buf25 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN );

	parser.parseLine( buf26 );
	parser.parseLine( buf27 );
	parser.parseLine( buf28 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SALE_ITEM_GOTTEN );
}

void CRubyCR0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "!Safe Loan       !    0.50";				// Safe Loan Gotten
	char buf2[] = "!Safe Loan       !    0.50";		
	char buf3[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";	
	char buf4[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten
	char buf5[] = "                              ";	
	char buf6[] = "       Thanks for shopping    ";			// Thank Gotten
	char buf7[] = "       Green Valley Grocery #2";	
	char buf8[] = "                              ";	
	char buf9[] = "1D32456092-001                ";	
	char buf10[] = "   Descr.     qty       amount";	
	char buf11[] = "-------       ---       ------";	
	char buf12[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";
	char buf13[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten

	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_SAFE_LOAN_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN );

	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_THANK_GOTTEN );

	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN );
}

void CRubyCR0SParserTest::testActualParse4()
{
	// state transform.

	char buf1[] = "!  Pay Out   !   15.00 #10";				// Safe Loan Gotten
	char buf2[] = "!  Pay Out   !   15.00 #10";		
	char buf3[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";	
	char buf4[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten
	char buf5[] = "                              ";	
	char buf6[] = "       Thanks for shopping    ";			// Thank Gotten
	char buf7[] = "       Green Valley Grocery #2";	
	char buf8[] = "                              ";	
	char buf9[] = "1D32456092-001                ";	
	char buf10[] = "   Descr.     qty       amount";	
	char buf11[] = "-------       ---       ------";	
	char buf12[] = "REG# 0001 CSH# 010 DR# 01  TRAN# 10872";
	char buf13[] = "02/04/04  11:51:33           ST# GVG02";// Date&Time Gotten

	CReceiverStub receiver;

	CRubyCR0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_PAY_OUT_GOTTEN );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN );

	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_THANK_GOTTEN );

	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN );
}



