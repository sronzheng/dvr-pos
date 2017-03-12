// Gilbarco0SParserTest.cpp: implementation of the CGilbarco0SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gilbarco0SParserTest.h"

#include "Gilbarco0SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CGilbarco0SParserTest );

CGilbarco0SParserTest::CGilbarco0SParserTest()
{

}

CGilbarco0SParserTest::~CGilbarco0SParserTest()
{

}

void CGilbarco0SParserTest::testSetItem1()
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// sale item. 
    // name:	SANTE FE R  $0.99,	amount:		8.99 dollar  

	char buf[] = "SANTE FE R  $8.99";

	CPPUNIT_ASSERT( parser.setItem( buf ) == GILBARCO0S_MAIN_ITEM );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "SANTE FE R" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 1 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 8 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 99 );
}

void CGilbarco0SParserTest::testSetItem2()
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// sale item. 
    // name:	SANTE FE R  $0.99,	amount:		8.99 dollar  
    // quantity:				1,	unit price:	8.99 dollar  

	char buf1[] = " 2 X       $3.550 ";
	char buf2[] = "SANTE FE R  $7.10";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) == GILBARCO0S_HEAD_ITEM );
	CPPUNIT_ASSERT( parser.setItem( buf2 ) == GILBARCO0S_MAIN_ITEM );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "SANTE FE R" )  == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 3 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 55 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 2 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 10 );
}

void CGilbarco0SParserTest::testSetItem3()
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// sale item. 
    // transaction should be informed only if the first sale time is received.

	char buf1[] = "SANTE FE R  $8.99";
	char buf2[] = "SANTE FE R  $8.99";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setItem( buf2 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );
}

void CGilbarco0SParserTest::testSetItem4()
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// next transation should be informed, if the sale item of next transaction received. 

	char buf1[] = "SANTE FE R  $8.99";
	char buf2[] = "TL/NOTAX     $6.08";

	CPPUNIT_ASSERT( parser.setItem( buf1 ) );
	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( parser.setSubtotal( buf2 ) );
	CPPUNIT_ASSERT( parser.m_transactionInformed == false );

}

void CGilbarco0SParserTest::testSetSubtotal1() 
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// subtotal. 
	// subtotal: 6.08 dollar
	char buf[] = "TL/NOTAX     $6.08";

	CPPUNIT_ASSERT( parser.setSubtotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_subtotal.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_subtotal.cent == 8 );
}

void CGilbarco0SParserTest::testSetTax1() 
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// subtotal. 
	// tax: 2.17 dorllar
	char buf[] = "TAX PD       $2.17";

	CPPUNIT_ASSERT( parser.setTax( buf ) );

	CPPUNIT_ASSERT( receiver.m_tax.dollar == 2 );
	CPPUNIT_ASSERT( receiver.m_tax.cent == 17 );
}

void CGilbarco0SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "TOTAL        $5.82";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 5 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 82 );
}

void CGilbarco0SParserTest::testSetPaymentType1() 
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// payment by cash
	// amount:	6.35 dollar  
	char buf[] = "CASH        $6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CGilbarco0SParserTest::testSetPaymentType2() 
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// payment by bank card
	// amount:	6.35 dollar  
	char buf[] = "DEBIT        $6.35";

	CPPUNIT_ASSERT( parser.setPaymentType( buf ) );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.dollar == 6 );
	CPPUNIT_ASSERT( receiver.m_paymentAmount.cent == 35 );
}

void CGilbarco0SParserTest::testSetChange1() 
{
	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	// change
    // amount: 7.53 dollar
	char buf[] = "CHANGE       $7.53";

	CPPUNIT_ASSERT( parser.setChange( buf ) );

	CPPUNIT_ASSERT( receiver.m_change.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_change.cent == 53 );
}

void CGilbarco0SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "PREPAID      PUMP3";	
	char buf2[] = "FUEL        $10.00";	// Item Gotten
	char buf3[] = "TOBACO      $0.69T";	// Item Gotten
	char buf4[] = "AGE 18 VERIFIED  ";	
	char buf5[] = " 2 X       $3.550";	
	char buf6[] = "MARLBORO-L  $7.10T";	// Item Gotten
	char buf7[] = "SUBTOT      $17.79 ";	
	char buf8[] = "TL/NOTAX    $17.79";	// Subtotal Gotten
	char buf9[] = "TAX PD       $0.25";	// Tax Gotten	
	char buf10[] = "TOTAL       $18.04";// Total Gotten
	char buf11[] = "CASH        $10.00";// Payment Type Gotten
	char buf12[] = "DEBIT       $10.00";// Payment Type Gotten
	char buf13[] ="CHANGE      $1.96";	// Change Gotten	

	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_BEGINNING );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 10 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );

	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 69 );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 7 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 10 );

	parser.parseLine( buf7 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf9 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf10 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf11 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf12 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == ATM );

	parser.parseLine( buf13 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_CHANGE_GOTTEN );
}

void CGilbarco0SParserTest::testActualParse2()
{
	// state transform.

	char buf1[] = " 2 X       $3.550";	
	char buf2[] = "MARLBORO-L  $7.10T";	// Item Gotten
	char buf3[] = "TL/NOTAX    $7.10";	// Subtotal Gotten
	char buf4[] = "TAX PD       $0.00";	// Tax Gotten	
	char buf5[] = "TOTAL       $7.10";// Total Gotten
	char buf6[] = "CASH        $7.10";// Payment Type Gotten
	char buf7[] = "                 ";	// Item Gotten
	char buf8[] = "FUEL        $10.00";	// Item Gotten

	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SUBTOTAL_GOTTEN );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_TAX_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_TOTAL_GOTTEN );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_paymentType == CASH );

	parser.parseLine( buf7 );
	parser.parseLine( buf8 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_SALE_ITEM_GOTTEN );
}

void CGilbarco0SParserTest::testActualParse3()
{
	// state transform.

	char buf1[] = "PAYIN       $62.29";	// Pay in Gotten
	char buf2[] = "REFERENCE NO";
	char buf3[] = "5905";	
	char buf4[] = "CASH        $62.29";	// Cash Gotten	

	CReceiverStub receiver;

	CGilbarco0SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_PAY_IN_GOTTEN );

	parser.parseLine( buf2 );
	parser.parseLine( buf3 );
	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == GILBARCO0S_STATE_CASH_GOTTEN );
}



