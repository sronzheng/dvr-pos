// StdOutputFake.cpp: implementation of the CReceiverStub class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReceiverStub::CReceiverStub()
 : m_CountOfSetItem(0),
   m_CountOfPayment(0),
   m_CountOfDateTime(0),
   m_countOfTransactions(0),
   m_year(0)
{
}

CReceiverStub::~CReceiverStub()
{
}

void CReceiverStub::setTransaction()
{
	m_countOfTransactions++;
}

void CReceiverStub::setTime( int year, 
							 int month, 
							 int day, 
							 int hour, 
							 int minute )
{
	m_CountOfDateTime++;

	m_year = year;
	m_month = month;
	m_day = day;
	m_hour = hour;
	m_minute = minute;
}

void CReceiverStub::setCardType( char typeName[] )
{
	m_cardType = typeName;
}

void CReceiverStub::setItem( char name[],
							 const MONEY_AMOUNT &unitPrice,
							 int quantity,					  
							 const MONEY_AMOUNT &price )
{
	m_CountOfSetItem++;

	m_itemQuantity = quantity;
	m_itemName = name;
	memcpy( &m_itemAmount, &price, sizeof(MONEY_AMOUNT) );
	memcpy( &m_unitAmount, &unitPrice, sizeof(MONEY_AMOUNT) );
}

void CReceiverStub::setItemVoided( const MONEY_AMOUNT &amount )
{
	memcpy( &m_amountVoided, &amount, sizeof( MONEY_AMOUNT ) );
}

void CReceiverStub::setSubtotal( const MONEY_AMOUNT &subtotal )
{
	memcpy( &m_subtotal, &subtotal, sizeof(MONEY_AMOUNT) );
}

void CReceiverStub::setTax( const MONEY_AMOUNT &tax )
{
	memcpy( &m_tax, &tax, sizeof(MONEY_AMOUNT) );
}

void CReceiverStub::setTotal( const MONEY_AMOUNT &total )
{
	memcpy( &m_total, &total, sizeof(MONEY_AMOUNT) );
}

void CReceiverStub::setPaymentType( PAYMENT_TYPE type, 
								    const MONEY_AMOUNT &amount )
{
	m_CountOfPayment++;

	m_paymentType = type;
	memcpy( &m_paymentAmount, &amount, sizeof(MONEY_AMOUNT));
}

void CReceiverStub::setChange( const MONEY_AMOUNT &change )
{
	memcpy( &m_change, &change, sizeof(MONEY_AMOUNT) );
}

void CReceiverStub::setRefund( const MONEY_AMOUNT &amount )
{
	memcpy( &m_refund, &amount, sizeof(MONEY_AMOUNT) );
}

void CReceiverStub::setTransactionVoided()
{
	m_transactionVoided = true;
}

