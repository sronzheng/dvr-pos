// Fujitsu1SParser.cpp: implementation of the CFujitsu1SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fujitsu1SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFujitsu1SParser::CFujitsu1SParser( CReceiver *receiver )
 : CParser( receiver ),
    m_state( FUJITSU1S_STATE_BEGINNING ),
   m_timeInformed( false )
{
}

CFujitsu1SParser::~CFujitsu1SParser()
{

}

void CFujitsu1SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case FUJITSU1S_STATE_BEGINNING:
		if( setTime( p ) )
			m_state = FUJITSU1S_STATE_DATE_TIME_GOTTEN;

		break;

	case FUJITSU1S_STATE_DATE_TIME_GOTTEN:
		if( strstr( p, "Safe Loan" ) != NULL ) 
				m_state = FUJITSU1S_STATE_SAFE_LOAN_GOTTEN;
		else if( strstr( p, "Paid Out" ) != NULL ) 
				m_state = FUJITSU1S_STATE_PAID_OUT_GOTTEN;
		else if( strstr( p, "Safe Drop" ) != NULL ) 
				m_state = FUJITSU1S_STATE_SAFE_DROP_GOTTEN;
		else
		{
			if( setItem( p ) )
				m_state = FUJITSU1S_STATE_SALE_ITEM_GOTTEN;
			else
				m_state = FUJITSU1S_STATE_DATE_TIME_EXPECTED;
		}

		break;

	case FUJITSU1S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "*VOID " ) != NULL ) 
		{
			if( setItemVoided( p ) )
				m_state = FUJITSU1S_STATE_SALE_ITEM_GOTTEN;
		}
		else if( strstr( p, "CANCELED" ) != NULL ) 
		{
			if( setTransactionVoided( p ) )
				m_state = FUJITSU1S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = FUJITSU1S_STATE_PAYMENT_TYPE_GOTTEN;
			else if( setItem( p ) )
				m_state = FUJITSU1S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case FUJITSU1S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "Subtotal:" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = FUJITSU1S_STATE_SUBTOTAL_GOTTEN;
		}
		else
		{	
			if( setPaymentType( p ) )
				m_state = FUJITSU1S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case FUJITSU1S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "Sales TAX:" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = FUJITSU1S_STATE_TAX_GOTTEN;
		}

		break;

	case FUJITSU1S_STATE_TAX_GOTTEN:
		if( strstr( p, "Total:" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = FUJITSU1S_STATE_TOTAL_GOTTEN;
		}

		break;

	case FUJITSU1S_STATE_TOTAL_GOTTEN:
		if( strstr( p, "Cash Change Due:" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = FUJITSU1S_STATE_CHANGE_GOTTEN;
		}
	
		break;

	case FUJITSU1S_STATE_CHANGE_GOTTEN:
	case FUJITSU1S_STATE_TRANSACTION_VOIDED_GOTTEN:
	case FUJITSU1S_STATE_SAFE_LOAN_GOTTEN:
	case FUJITSU1S_STATE_PAID_OUT_GOTTEN:
	case FUJITSU1S_STATE_SAFE_DROP_GOTTEN:
		if( strstr( p, "Transaction " ) != NULL ) 
		{
			if( setEndFlag( p ) )
				m_state = FUJITSU1S_STATE_END_FLAG_GOTTEN;
		}

		break;

	default:
		//never go here.
		break;
	}
}

bool CFujitsu1SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// e2s82t1   03/03/04 05:58:27

	// look for month
	char *pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	p = pSlash - 2;
	m_month = atoi( p );

	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	m_day = atoi( p );

	p = pSlash + 1;

	// look for year
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	m_year = atoi( p ) + 2000;

	p = pSlash + 1;

	// look for hour
	p = nextDigit( p );
	if( p == NULL )
		return false;

	pSlash = strchr( p, ':' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	m_hour = atoi( p );

	p = pSlash + 1;

	// look for minute
	pSlash = strchr( p, ':' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	m_minute = atoi( p );

	p = pSlash + 1;

	return true;
}

bool CFujitsu1SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// LIFESAVERS P                     0.69
	
	// EXAMPLE 2: 
	// REESES CUPS                    --0.69

	// EXAMPLE 3: 
	// SWISHERSWEET       2 @   0.69    1.38T
	
	char *pStart = p;

	p = p + strlen( p );
	
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	//look for item price.
	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT price;
	if( getMoneyAmount( p+1, price ) == NULL )
		return false;

	MONEY_AMOUNT unitPrice;
	int quantity;

	// if unit price and quantity part exist, look for them, or else set unit price 
	// equal to price, set quantity to be 0. 
	*p = 0;
	char *pSlash = strrchr( pStart, '@' );
	if( pSlash == NULL )
	{
		quantity = 1;
		memcpy( &unitPrice, &price, sizeof(MONEY_AMOUNT) );
	}
	else
	{
		// look for item unit price
		p = nextDigit( pSlash+1 );
		if( p == NULL )
			return false;
		
		if( getMoneyAmount( p, unitPrice ) == NULL )
			return false;
		
		// look for quantity
		p = preDigit( pSlash, pStart );
		if( p == NULL )
			return false;
		
		*(p+1) = 0;
		
		p = strrchr( pStart, ' ');
		if( p == NULL )
			return false;
		
		quantity = atoi( p+1 );
	}
	
	//look for item name
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	if( ! m_timeInformed )
	{
		m_receiver->setTransaction(); 
		m_receiver->setTime( m_year, m_month, m_day, m_hour, m_minute ); 

		m_timeInformed = true;
	}

	m_receiver->setItem( pStart, unitPrice, quantity, price );  

	return true;
}

bool CFujitsu1SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Subtotal:                5.96

	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setSubtotal( amount ); 

	return true;
}

bool CFujitsu1SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Sales TAX:                 0.10

	// look for tax
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setTax( amount ); 

	return true;
}

bool CFujitsu1SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Total:              1.48
           
	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setTotal( amount ); 

	return true;
}

bool CFujitsu1SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Cash Change Due:                18.52

	// look for change
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setChange( amount ); 

	return true;
}

bool CFujitsu1SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		//
		// don't discard the space characters at tail, 
		// for differing it with "Cash Back" item.
		//
		"Cash   ",		CASH,				// Cash          20.00
		"Credit Card",	CREDIT,				// Credit Card    5.96
		"Debit Card",	ATM,				// Debit Card    21.82
	};

	// look for amount. 
	// if payment type matched, handle it, and stop search routine
	for ( int i = 0; i < sizeof( paymentType ) / sizeof( paymentType[0] ); i++ )
	{	
		char *pStr = strstr( p, paymentType[i].key );

		if ( pStr != NULL )
		{
			// look for amount
			p = nextDigit( p );
			if( p == NULL )
				return false;
			
			MONEY_AMOUNT amount;
			if( getMoneyAmount( p, amount ) == NULL )
				return false;
			
			m_receiver->setPaymentType( paymentType[i].type, amount );  

			return true;
		}
	}

	return false;
}

bool CFujitsu1SParser::setItemVoided( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// *VOID 24 OZ COFFEE*             -1.19T
	
	char *pStart = p;

	p = p + strlen( p );
	
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	//look for item price.
	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p+1, amount ) == NULL )
		return false;

	m_receiver->setItemVoided( amount );  

	return true;
}

bool CFujitsu1SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	// CANCELED

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}

bool CFujitsu1SParser::setEndFlag( char *buffer )
{
	m_timeInformed = false;

	return true;
}
