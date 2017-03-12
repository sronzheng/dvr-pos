// StreamReceiverTest.cpp: implementation of the CStreamReceiverTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StreamReceiverTest.h"

#include "binarystream.h"
#include "streamreceiver.h"

#include <time.h>
#include <stdio.h>

#include "posstruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPPUNIT_TEST_SUITE_REGISTRATION( CStreamReceiverTest );

CStreamReceiverTest::CStreamReceiverTest()
{

}

CStreamReceiverTest::~CStreamReceiverTest()
{

}

void CStreamReceiverTest::testSetTransaction1()
{
	// Transaction
	// keyword: "Transaction"
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	receiver.setTransaction( );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Transaction" ) == 0 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetTime1()
{
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	receiver.setTime( 2006, 1, 24, 13, 50 );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "DateTime" ) == 0 );

	UINT64 nano100second  = data->dateTime * 10000;

	FILETIME fileTime;

	fileTime.dwHighDateTime = nano100second >> 32;
	fileTime.dwLowDateTime = nano100second & 0xFFFFFFFF;

	SYSTEMTIME systemTime;
	FileTimeToSystemTime( &fileTime, &systemTime);

	CPPUNIT_ASSERT( systemTime.wYear == 2006 ); 
	CPPUNIT_ASSERT( systemTime.wMonth == 1 ); 
	CPPUNIT_ASSERT( systemTime.wDay == 24 ); 
	CPPUNIT_ASSERT( systemTime.wHour == 13 ); 
	CPPUNIT_ASSERT( systemTime.wMinute == 50 ); 

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetItem1()
{
	//set item 
	// keyword: "Item",			item name: "TEST SKU"
	// unit price: 2.01 dollar	quantity: 2
	// price: 4.02 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT unitPrice;
	unitPrice.dollar = 2;
	unitPrice.cent = 1;

	MONEY_AMOUNT price;
	price.dollar = 4;
	price.cent = 2;

	receiver.setItem( "TEST SKU", unitPrice, 2, price );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Item" ) == 0 );

	CPPUNIT_ASSERT( strcmp( data->itemData.name, "TEST SKU" ) == 0 );

	CPPUNIT_ASSERT( data->itemData.unitPrice == 201 );

	CPPUNIT_ASSERT( data->itemData.quantity == 2 );

	CPPUNIT_ASSERT( data->itemData.price == 402 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetSubtotal1()
{
	//set subtotal 
	// keyword: "Subtotal",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setSubtotal( amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Subtotal" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetTotal1()
{
	//set total 
	// keyword: "Total",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setTotal( amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Total" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetTax1()
{
	//set tax 
	// keyword: "Tax",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setTax( amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Tax" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetPaymentType1()
{
	// payment by cash
	// keyword: "Cash",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setPaymentType( CASH, amount );

	char buf[128];

	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Cash" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetPaymentType2()
{
	// payment by check
	// keyword: "Check",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setPaymentType( CHECK, amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Check" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetPaymentType3()
{
	// payment by credit card
	// keyword: "Credit",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setPaymentType( CREDIT, amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Credit" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetPaymentType4()
{
	// payment by ATM card
	// keyword: "ATM",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setPaymentType( ATM, amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "ATM" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetPaymentType5()
{
	// payment by gift certificate 
	// keyword: "Gift certificate",		amount: 2.01 dollar
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setPaymentType( GIFT_CERTIFICATE, amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Gift certificate" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetChange1()
{
	// set change
	// keyword: "Change"
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setChange( amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Change" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetItemVoided1()
{
	// item voided
	// keyword: "Item Voided"
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setItemVoided( amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Item Voided" ) == 0 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetRefund1()
{
	// set refund
	// keyword: "Refund"
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	MONEY_AMOUNT amount;
	amount.dollar = 2;
	amount.cent = 1;

	receiver.setRefund( amount );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Refund" ) == 0 );

	CPPUNIT_ASSERT( data->amount == 201 );

	CloseHandle( mutex );
}

void CStreamReceiverTest::testSetTransactionVoided1()
{
	// Transaction voided
	// keyword: "Transaction Voided"
	HANDLE mutex = CreateMutex( NULL, FALSE, NULL );
	CBinaryStream stream( mutex );

	CStreamReceiver receiver( stream );

	receiver.setTransactionVoided( );

	char buf[128];
	CPPUNIT_ASSERT( stream.read( buf, sizeof(buf) ) == sizeof( POS_DATA ) ); 

	POS_DATA *data = (POS_DATA *)buf;
	CPPUNIT_ASSERT( strcmp( data->keyword, "Transaction Voided" ) == 0 );

	CloseHandle( mutex );
}