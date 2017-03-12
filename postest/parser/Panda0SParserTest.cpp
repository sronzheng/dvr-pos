// Panda0SParserTest.cpp: implementation of the CPanda0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Panda0SParserTest.h"

#include "Panda0Sparser.h"
#include "receiverstub.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CPanda0SParserTest );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanda0SParserTest::CPanda0SParserTest()
{

}

CPanda0SParserTest::~CPanda0SParserTest()
{

}

void CPanda0SParserTest::testTestTime1()
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// data & time
	// 05-27-2005, 10:28
	char buf[] = "v 129 10:28 #08 MAY.27'05  REG0001";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 5 );
	CPPUNIT_ASSERT( receiver.m_day == 27 );
	CPPUNIT_ASSERT( receiver.m_hour == 10 );
	CPPUNIT_ASSERT( receiver.m_minute == 28 );
}

void CPanda0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// sale item. 
    // name:		S DOMSTCBEER,	amount:		8.99 dollar  

	char buf[] = "\x1Bv  1 S DOMSTCBEER              8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "S DOMSTCBEER" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CPanda0SParserTest::testSetItem2()
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// sale item. 
    // name:		S DOMSTCBEER,	amount:		0.99 dollar  

	char buf[] = "\x1Bv  1 S DOMSTCBEER              .99";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "S DOMSTCBEER" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CPanda0SParserTest::testSetItem3()
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

    // transaction should be informed only if the first sale time is received.

	char buf1[] = "\x1Bv  1 S DOMSTCBEER              8.99";
	char buf2[] = "\x1Bv  1 S DOMSTCBEER              8.99";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );
}

void CPanda0SParserTest::testSetItem4()
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// next transation should be informed, if the sale item of next transaction received. 

	char buf1[] = "\x1Bv  1 S DOMSTCBEER              8.99";
	char buf2[] = "TAX               1.26";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setTax( buf2 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == false );

}

void CPanda0SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// tax. 
	// tax: 1.26 dorllar
	char buf[] = "TAX               1.26";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 26 );
}

void CPanda0SParserTest::testSetTax2() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// tax. 
	// tax: 0.26 dorllar
	char buf[] = "TAX               .26";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 26 );
}

void CPanda0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "TOTL          ! 5.82!";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CPanda0SParserTest::testSetTotal2() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// total. 
	// total:	0.82 dollar  
	char buf[] = "TOTL          ! .82!";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CPanda0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = " CASH             6.35  ";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CPanda0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// payment by cash
	// amount:	0.35 dollar  
	char buf[] = " CASH             .35  ";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CPanda0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHNG             7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CPanda0SParserTest::testSetChange2() 
{
	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	// change
    // amount: 0.53 dollar
	char buf[] = "CHNG             .53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CPanda0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "\x1Bv  1 S DOMSTCBEER              2.00";	// Item Gotten
	char buf2[] = "\x1Bv  1 L DOMST BEER                  ";		
	char buf3[] = "\x1Bv  1 BOTL    WATER             1.49";	// Item Gotten	
	char buf4[] = "\x1Bv  1 ORANGE  JUICE             1.69";	// Item Gotten
	char buf5[] = "\x1Bv         -------------------------";	
	char buf6[] = "                 TAX                .27";	// Tax Gotten	
	char buf7[] = "                 TOTL          !  5.45!";	// Total Gotten
	char buf8[] = "                 CASH              5.00";	// Payment Type Gotten
	char buf9[] = "                 CASH               .50";	// Payment Type Gotten	
	char buf10[] = "                CHNG               .05";	// Change Gotten
	char buf11[] = " HELP US TO BETTER SERVE YOU          ";		
	char buf12[] = " CONTACT PANDA AT 800-877-8988        ";		
	char buf13[] = " WE DO CATERING  949-837-7058         ";	
	char buf14[] = "                                      ";	
	char buf15[] = "\x1Bv 129 10:28 #08 MAY.27'04  REG0001";	// Time Gotten

	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( strcmp( receiver.m_itemName.c_str(), "S DOMSTCBEER" ) == 0 );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( strcmp( receiver.m_itemName.c_str(), "S DOMSTCBEER" ) == 0 );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( strcmp( receiver.m_itemName.c_str(), "BOTL    WATER" ) == 0 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_SALE_ITEM_GOTTEN );
	
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 0 );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 50 );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_CHANGE_GOTTEN );

	parser.parseLine( buf11 );
	parser.parseLine( buf12 );
	parser.parseLine( buf13 );
	parser.parseLine( buf14 );
	parser.parseLine( buf15 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_DATE_TIME_GOTTEN );
}

void CPanda0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "\x1Bv  1 S DOMSTCBEER              1.98";	// Item Gotten
	char buf2[] = "\x1Bv         -------------------------";	
	char buf3[] = "                 TAX                .02";	// Tax Gotten
	char buf4[] = "                 TOTL          !  2.00!";	// Total Gotten
	char buf5[] = "                 CASH              2.00";	// Payment Type Gotten
	char buf6[] = " HELP US TO BETTER SERVE YOU          ";		
	char buf7[] = " CONTACT PANDA AT 800-877-8988        ";		
	char buf8[] = " WE DO CATERING  949-837-7058         ";	
	char buf9[] = "                                      ";	
	char buf10[] = "\x1Bv 129 10:28 #08 MAY.27'04  REG0001";	// Time Gotten

	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_PAYMENT_TYPE_GOTTEN );

	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	parser.parseLine( buf9 );
	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_DATE_TIME_GOTTEN );
}

void CPanda0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "\x1Bv **REGISTER**             X2    6432";	// Register Gotten
	char buf2[] = "      -----------------------------------";	
	char buf3[] = "       FOOD          0                .00";	
	char buf4[] = "       DRINK         0                .00";	
	char buf5[] = "      ===================================";	
	char buf6[] = "                                         ";	
	char buf7[] = "\x1Bv 129 10:28 #08 MAY.27'04  REG0001  ";	// Time Gotten

	CReceiverStub receiver;

	CPanda0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_REGISTER_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == PANDA0S_STATE_DATE_TIME_GOTTEN );
}



