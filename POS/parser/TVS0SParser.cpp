// TVS0SParser.cpp: implementation of the CTVS0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TVS0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTVS0SParser::CTVS0SParser( CReceiver *receiver )
 : CParser( receiver, '\xC8' ),			// line separator: '\xC8'
   m_state( TVS0S_STATE_BEGINNING ),
   m_transactionInformed( false )
{
}

CTVS0SParser::~CTVS0SParser()
{

}

void CTVS0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case TVS0S_STATE_BEGINNING:
		if( strstr( p, "SAFE LOAN" ) != NULL )
			m_state = TVS0S_STATE_SAFE_LOAN_GOTTEN;
		else if( strstr( p, "PAY OUT" ) != NULL )
			m_state = TVS0S_STATE_PAY_OUT_GOTTEN;
		else if( strstr( p, "NO SALE" ) != NULL )
			m_state = TVS0S_STATE_NO_SALE_GOTTEN;
		else 
		{
			if( setItem( p ) )
				m_state = TVS0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case TVS0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "TAX" ) != NULL )
		{
			if( setTax( p ) )
				m_state = TVS0S_STATE_TAX_GOTTEN;
		}else
		{
			setItem( p );

			// keep state
		}

		break;

	case TVS0S_STATE_TAX_GOTTEN:
		if( setTotal( p ) )
			m_state = TVS0S_STATE_TOTAL_GOTTEN;

		break;

	case TVS0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = TVS0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case TVS0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( setTime( p ) )
			m_state = TVS0S_STATE_DATE_TIME_GOTTEN;

		break;

	case TVS0S_STATE_SAFE_LOAN_GOTTEN:
	case TVS0S_STATE_PAY_OUT_GOTTEN:
	case TVS0S_STATE_NO_SALE_GOTTEN:
		if( setTime( p, false ) )
			m_state = TVS0S_STATE_DATE_TIME_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CTVS0SParser::setTime( char *buffer, bool inform )
{
	char *p = buffer;

	// EXAMPLE:
	// 12/13/05 14:15         1TI

	p = nextDigit( p );
	if( p == NULL )
		return false;

	// look for month
	char *pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return NULL;
	
	*pSlash = 0;
	int month = atoi( p );
	
	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return NULL;
	
	*pSlash = 0;
	int day = atoi( p );
	
	p = pSlash + 1;

	// look for year
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL)
		return NULL;
	
	*pSlash = 0;
	int year = atoi( p ) + 2000;
	
	p = pSlash + 1;

	// look for hour
	pSlash = strchr( p, ':' );
	if( pSlash == NULL)
		return NULL;

	*pSlash = 0;
	int	hour = atoi( p );

	p = pSlash + 1;
	
	// look for minute
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL)
		return NULL;

	*pSlash = 0;
	int minute = atoi( p );

	if( inform )
		m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CTVS0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// for simplifyinig the following code,
	// replace '\r', '\n' with ' '.
	char *pChar = buffer;
	while( *pChar != '\0' )
	{
		if( *pChar == '\r' || *pChar == '\n' )
			*pChar = ' ';
		pChar++;
	}

	// EXAMPLE 1: sale item
	// T  BLACK V 04246   899 1
	
	// EXAMPLE 2: sale item
	// PRO               -045 1

	// EXAMPLE 3: invalid item
	// ( the following line ,inserted between sale item lines, is invalid.
	//   ignore it. )
    //              2000 1YY

	char *pStart = p;

	//if it is a valid item, ignore it and return.
	p = p + strlen( p );
	if( strcmp( p-2, "YY" ) == 0 )
		return false;

	//skip last digit
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*p = 0;
	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	// look for price
	p = preDigit( p, pStart );
	*(p+1) = 0;

	while( *(p-1) >= '0' && *(p-1) <= '9' )
		p--;

	int amt = atoi( p );

	MONEY_AMOUNT price;
	price.dollar = amt / 100;
	price.cent = amt % 100;

	// check sign of price amount.
	if( *(p-1) == '-' )
	{
		price.dollar	= -price.dollar;
		price.cent		= -price.cent;

		p--;
	}

	// try to find a digit or a item name.
	while ( *(p-1) == ' ' )
		p--;

	//
	// If a digit found, skip it and the space characters before it.  
	//
	// Is it a part of sale time? To be honest, I don't know.
	// I have to assume that it is NOT. Of course, it may be a problem!
	//
	if( *(p-1) >= '0' && *(p-1) <= '9' )
	{
		*p = 0;
		p = strrchr( pStart, ' ');
		if( p == NULL ) 
			return false;

		while ( *(p-1) == ' ' )
			p--;
	}

	*p = 0;

	MONEY_AMOUNT unitPrice;
	unitPrice.dollar = 0;
	unitPrice.cent = 0;

	if( ! m_transactionInformed )
	{
		m_receiver->setTransaction();

		m_transactionInformed = true;
	}

	m_receiver->setItem( pStart, unitPrice, 0, price );   

	return true;
}

bool CTVS0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TAX                045 1TX

	//look for tax
	p = nextDigit( p );
	if( p == NULL )
		return false;

	int amt = atoi( p );

	MONEY_AMOUNT amount;
	amount.dollar	= amt / 100;
	amount.cent		= amt % 100;

	m_receiver->setTax( amount ); 

	m_transactionInformed = false;

	return true;
}

bool CTVS0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL             3175 1TT

	//look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	int amt = atoi( p );

	MONEY_AMOUNT amount;
	amount.dollar	= amt / 100;
	amount.cent		= amt % 100;

	m_receiver->setTotal( amount ); 

	return true;
}

bool CTVS0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",		CASH,		//CASH            2100   1AM
		"CREDIT",	CREDIT,		//CREDIT          3175   1CG				
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
			if ( !p ) 
				return false;

			int amt = atoi( p );

			MONEY_AMOUNT amount;
			amount.dollar	= amt / 100;
			amount.cent		= amt % 100;	 

			m_receiver->setPaymentType( paymentType[i].type, amount);  
	
			return true;
		}
	}

	return false;
}
