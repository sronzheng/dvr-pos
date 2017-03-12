// Casio6100A0SParserTest.cpp: implementation of the CCasio6100A0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Casio6100A0SParserTest.h"

#include "Casio6100A0SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CCasio6100A0SParserTest );

CCasio6100A0SParserTest::CCasio6100A0SParserTest()
{

}

CCasio6100A0SParserTest::~CCasio6100A0SParserTest()
{

}

void CCasio6100A0SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = "REG            13-12-2005(SAT)  14:15 ";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CCasio6100A0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	// sale item. 
    // name:		FOOD,	amount:		8.99 dollar  

	char buf[] = " 1  FOOD                     8.99 ";

	CPPUNIT_ASSERT( parser.setItem( buf ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "FOOD" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CCasio6100A0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	// total. 
	// 8.70 dollar  
	char buf[] = "      TL               !! 8.70!";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 70 );
}

void CCasio6100A0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = " CASH                    6.35 ";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CCasio6100A0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = " CG                       7.53 ";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CCasio6100A0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "REG            17-05-2003(SAT)  13:01 ";	// Time Gotten		
	char buf2[] = "!!C!01       MC#01              000329";
	char buf3[] = "                                 CT  1";
	char buf4[] = "   1  FOOD                      1.00  ";	// Item Gotten
	char buf5[] = "   1  DRINKS                    2.00  ";	// Item Gotten
	char buf6[] = "                                      ";		
	char buf7[] = "     TL                  .!! 6.00.!.  ";	// Total Gotten
	char buf8[] = "    CASH                    10.00     ";	// Payment Type Gotten
	char buf9[] = "      CG                     4.00     ";	// Change Gotten

	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );

	parser.parseLine( buf6 );
	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_CHANGE_GOTTEN );
}

void CCasio6100A0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = "REG            17-05-2003(SAT)  13:01 ";	// Time Gotten		
	char buf2[] = "!!C!01       MC#01              000329";
	char buf3[] = "                                 CT  1";
	char buf4[] = "   1  DRINKS                    6.00  ";	// Item Gotten
	char buf5[] = "                                      ";		
	char buf6[] = "     TL                  .!! 6.00.!.  ";	// Total Gotten
	char buf7[] = "    CASH                     5.00     ";	// Payment Type Gotten
	char buf8[] = "    CASH                     1.00     ";	// Payment Type Gotten
	char buf9[] = "REG            17-05-2003(SAT)  13:01 ";	// Time Gotten		

	CReceiverStub receiver;

	CCasio6100A0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf5 );
	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 5 );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 1 );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == CASIO6100A0S_STATE_DATE_TIME_GOTTEN );
}




