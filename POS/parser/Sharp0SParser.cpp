// Sharp0SParser.cpp: implementation of the CSharp0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sharp0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharp0SParser::CSharp0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( SHARP0S_STATE_BEGINNING )
{
}

CSharp0SParser::~CSharp0SParser()
{
}

void CSharp0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case SHARP0S_STATE_PAYMENT_TYPE_GOTTEN:

		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
			{
				m_state = SHARP0S_STATE_CHANGE_GOTTEN;
				break;
			}
		}
		else
		{	
			if( setPaymentType( p ) )
			{
				m_state = SHARP0S_STATE_PAYMENT_TYPE_GOTTEN;
				break;
			}
		}

		// go through.
		// if it is not change item or payment type item, 
		// try to handle it as a data item.

		//break;

	case SHARP0S_STATE_BEGINNING:
		if( setDate( p ) )
			m_state = SHARP0S_STATE_DATE_GOTTEN;

		break;

	case SHARP0S_STATE_DATE_GOTTEN:
		if( setTime( p ) )
			m_state = SHARP0S_STATE_TIME_GOTTEN;

		break;

	case SHARP0S_STATE_TIME_GOTTEN:
		if( strstr( p, "NO SALE" ) != NULL ) 
			m_state = SHARP0S_STATE_NO_SALE_GOTTEN;
		else 
		{
			if( setPaymentType( p ) )
				m_state = SHARP0S_STATE_PAYMENT_TYPE_GOTTEN;
			else if( setItem( p ) )
				m_state = SHARP0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case SHARP0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "MDSE ST" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = SHARP0S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, "***TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = SHARP0S_STATE_TOTAL_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = SHARP0S_STATE_PAYMENT_TYPE_GOTTEN;
			else if( setItem( p ) )
				m_state = SHARP0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case SHARP0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "TAX1" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = SHARP0S_STATE_TAX_GOTTEN;
		}

		break;

	case SHARP0S_STATE_TAX_GOTTEN:
		if( strstr( p, "***TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = SHARP0S_STATE_TOTAL_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = SHARP0S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case SHARP0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = SHARP0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CSharp0SParser::setDate( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// c4 \xA 04/22/04\x1D!         000004
	// #0021      2:03AM SERV.0040004

	// look for month
	char *pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	p = pSlash;
	while( *(p-1) >= '0' && *(p-1) <= '9'  )
		p--;

	if( p == pSlash )
		return false;
	
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
	while( *(pSlash+1) >= '0' && *(pSlash+1) <= '9' )
		pSlash++;
	if( pSlash == p )
		return false;
	
	*(pSlash+1) = 0;
	m_year = atoi( p ) + 2000;

	return true;
}

bool CSharp0SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// c4 \xA 04/22/04\x1D!         000004
	// #0021      2:03AM SERV.0040004

	// look for the start position of time
	p = nextDigit( p );
	if( p == NULL )
		return false;

	p = strchr( p, ' ');
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	// look for hour
	char *pSlash = strchr( p, ':' );
	if( pSlash == NULL)
		return NULL;

	*pSlash = 0;
	int	hour = atoi( p );

	p = pSlash + 1;

	char *pMinute = p;

	// check if it is a morning time, or afternoon time.
	while( *p >= '0' && *p <= '9' ) 
		p++;
	
	bool morning;
	if( strncmp( p, "AM", 2) == 0 )
		morning = true;
	else if( strncmp( p, "PM", 2) == 0 )
		morning = false;
	else
		return false;

	if( ! morning )
		hour += 12;

	// look for minute
	*p = 0;
	int minute = atoi( pMinute );

	m_receiver->setTransaction();

	m_receiver->setTime( m_year, m_month, m_day, hour, minute ); 

	return true;
}

bool CSharp0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// ! 6 DONUTS               $3.59
	
	// EXAMPLE 2: sale item
	// ! FREE DON W-COF         -0.59

	// EXAMPLE 3: item voided
	// ! 1 DONUT                 V-0.70

	// EXAMPLE 4: item prefix. ignore it.
    // !                     6/ $3.59

	// look for the start position of amount

	char *pStart = p;

	char *pSlash = strchr( p, '.' );
	if( pSlash == NULL )
		return false;

	p = pSlash; 
	while( *(p-1) >= '0' && *(p-1) <= '9' )
		p--;

	if( p == pSlash )
		return false;

	// look for price
	MONEY_AMOUNT price;

	if( getMoneyAmount( p, price) == NULL )
		return false;

	// look for sign of price.
	// if the digit don't begin with '$' or '-', it is not a sale item.
	if( *(p-1) == '-' )
	{
		price.dollar = -price.dollar;
		price.cent	 = -price.cent;

		// if it is a voided item, handle it and return 
		if( *(p-2) == 'V' || *(p-2) == 'R' )
		{
			m_receiver->setItemVoided( price );

			return true;
		}

	}else if( *(p-1) == '$' )
	{
		// if it is a item prefix, do nothing and return.
		if( *(p-2) == ' ' && *(p-3) == '/')
			return false;
	}
	else 
		return false;

	p = p - 2;

	// look for item name
	while( *p == ' ' )
		p--;

	*(p+1) = 0;

	p = strrchr( pStart, '!');
	if( p == NULL )
		return false;

	while( *(p+1) == ' ' )
		p++;
	
	MONEY_AMOUNT unitPrice;
	memset( &unitPrice, 0, sizeof(MONEY_AMOUNT));

	m_receiver->setItem( p+1, unitPrice, 0, price ); 

	return true;
}

bool CSharp0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// MDSE ST                $6.08

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

bool CSharp0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TAX1                   $0.13

	// EXMAPLE 2:
	// TAX1                   -0.08

	p = strstr( p, "TAX1" );
	if( p == NULL )
		return false;

	p = p + 4;

	// look for tax
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	// look for sign of tax amount.
	if( *(p-1) == '-' )
	{
		amount.dollar	= -amount.dollar;
		amount.cent		= -amount.cent;
	}

	m_receiver->setTax( amount ); 

	return true;
}

bool CSharp0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// ***TOTAL            !$5.82!

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

bool CSharp0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	// I don't know how handle pay by charge, so return now.
	if( strstr( p, "CHARGE" ) != NULL )
		return true;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"ASH",		CASH,				// C.! ASH               !$6.35!
		"GIFT RED",	GIFT_CERTIFICATE,	// GIFT RED               $5.82
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

bool CSharp0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE                   $7.53

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