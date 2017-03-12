// RubyCR0SParser.cpp: implementation of the CRubyCR0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RubyCR0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRubyCR0SParser::CRubyCR0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( RUBYCR0S_STATE_BEGINNING ),
   m_transactionInformed( false )
{
	m_preItemName[0] = 0;
	memset( &m_preItemAmount, 0, sizeof(MONEY_AMOUNT));
	memset( &m_preProAmount, 0, sizeof(MONEY_AMOUNT));

	m_prePaymentType = CASH;
	memset( &m_prePaymentAmount, 0, sizeof(MONEY_AMOUNT));
}

CRubyCR0SParser::~CRubyCR0SParser()
{

}

void CRubyCR0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN:
		if( strstr( p, "Thanks for shopping" ) != NULL ) 
		{
			m_state = RUBYCR0S_STATE_THANK_GOTTEN;
			
			break;
		}

		// go through.
		// if it is not thank item, try to handle it as a data item.

		//break;

	case RUBYCR0S_STATE_BEGINNING:
		if( strstr( p, "** ERROR CORRECTION **" ) != NULL ) 
			m_state = RUBYCR0S_STATE_ERROR_HEAD_GOTTEN;
		else if( strstr( p, "Safe Loan" ) != NULL ) 
			m_state = RUBYCR0S_STATE_SAFE_LOAN_GOTTEN;
		else if( strstr( p, "Pay Out" ) != NULL ) 
			m_state = RUBYCR0S_STATE_PAY_OUT_GOTTEN;
		else
		{
			if( setItem( p ) )
				m_state = RUBYCR0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case RUBYCR0S_STATE_ERROR_HEAD_GOTTEN:
		if( strstr( p, "******************************" ) != NULL ) 
			m_state = RUBYCR0S_STATE_ERROR_TAIL_GOTTEN;

		break;

	case RUBYCR0S_STATE_ERROR_TAIL_GOTTEN:
		if( strstr( p, "Sub Total" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = RUBYCR0S_STATE_SUBTOTAL_GOTTEN;
		}
		else
		{
			if( setItem( p ) )
				m_state = RUBYCR0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case RUBYCR0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "** ERROR CORRECTION **" ) != NULL ) 
			m_state = RUBYCR0S_STATE_ERROR_HEAD_GOTTEN;
		else if( strstr( p, "Sub Total" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = RUBYCR0S_STATE_SUBTOTAL_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state
		}

		break;

	case RUBYCR0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "Tax" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = RUBYCR0S_STATE_TAX_GOTTEN;
		}

		break;

	case RUBYCR0S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = RUBYCR0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case RUBYCR0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "Change" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = RUBYCR0S_STATE_CHANGE_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN;
			else if( setTime( p ) )
				m_state = RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN;
		}


		break;

	case RUBYCR0S_STATE_CHANGE_GOTTEN:
		if( setTime( p ) )
			m_state = RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN;

		break;

	case RUBYCR0S_STATE_THANK_GOTTEN:
		if( setTime( p, false ) )
			m_state = RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN;

		break;

	case RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN:
		if( strstr( p, "Thanks for shopping" ) != NULL ) 
		{
			m_state = RUBYCR0S_STATE_THANK_GOTTEN;

			memset( &m_preItemAmount, 0, sizeof(MONEY_AMOUNT) );
			m_preItemName[0] = 0;

			memset( &m_preProAmount, 0, sizeof(MONEY_AMOUNT) );

			memset( &m_prePaymentAmount, 0, sizeof(MONEY_AMOUNT) );
			m_prePaymentType = CASH;
		}

		break;

	case RUBYCR0S_STATE_PAY_OUT_GOTTEN:
	case RUBYCR0S_STATE_SAFE_LOAN_GOTTEN:
		if( setTime( p, false ) )
			m_state = RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}

}

bool CRubyCR0SParser::setTime( char *buffer, bool inform)
{
	char *p = buffer;

	// EXAMPLE:
	// 02/04/05  09:16:47           ST# GVG02

	// look for month
	p = nextDigit( p );
	if( p == NULL ) 
		return false;

	char *pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;
	
	*pSlash = 0;
	int month = atoi( p );
	
	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;
	
	*pSlash = 0;
	int day = atoi( p );
	
	p = pSlash + 1;

	// look for year
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL)
		return false;
	
	*pSlash = 0;
	int year = atoi( p ) + 2000;
	
	p = pSlash + 1;

	// look for hour
	p = nextDigit( p );
	if( p == NULL ) 
		return false;

	pSlash = strchr( p, ':' );
	if( pSlash == NULL)
		return false;

	*pSlash = 0;
	int	hour = atoi( p );

	p = pSlash + 1;
	
	// look for minute
	pSlash = strchr( p, ':' );
	if( pSlash == NULL)
		return false;

	*pSlash = 0;
	int minute = atoi( p );

	if( inform )
		m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CRubyCR0SParser::setProItem( char *buffer )
{
	char *p = buffer;

	// EXAMPLE 1: discount item
	// \x1         Pro           -0.15

	// look for price.
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	if( *(p-1) == '-' )
	{
		amount.dollar	= -amount.dollar;
		amount.cent		= -amount.cent;
	}

	// RubyCR0S may send a sale item two times. It's too bad !
	// I must discard the second one.
	if( memcmp( &m_preProAmount, &amount, sizeof(MONEY_AMOUNT) ) == 0 )
		memset( &m_preProAmount, 0, sizeof(MONEY_AMOUNT) );
	else
	{
		memcpy( &m_preProAmount, &amount, sizeof(MONEY_AMOUNT) );

		MONEY_AMOUNT unitPrice;
		unitPrice.dollar = 0;
		unitPrice.cent = 0;

		if( ! m_transactionInformed )
		{
			m_receiver->setTransaction(); 

			m_transactionInformed = true;
		}
		
		m_receiver->setItem( "Pro", unitPrice, 0, amount );   
	}

	return true;
}

bool CRubyCR0SParser::setItem( char *buffer )
{
	char *p = buffer;

	// EXAMPLE 1: sale item
	// \x1 T  BUD 12P 00333   1   8.99

	// EXAMPLE 2: discount item
	// \x1         Pro           -0.15

	char *pStart = p;

	// if it is a discount item, handle it and return.
	if( strstr( p, " Pro ") != NULL )
		return setProItem( p );

	p = p + strlen( p );

	// look for price
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	p = strrchr( pStart, ' ' );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p+1, amount ) == NULL )
		return false;

	// skip two digits
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	// look for item name.
	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	while ( *p == ' ' )
		p--;

	*(p+1) = 0;

	p = strrchr( pStart, '\x1');
	if( p == NULL )
		return false;

	char *name = p + 1;

	// RubyCR0S may send a sale item two times. It's too bad !
	// I must discard the second one.
	if( strcmp( m_preItemName, name ) == 0 && 
		memcmp( &m_preItemAmount, &amount, sizeof(MONEY_AMOUNT) ) == 0 )
	{
		memset( &m_preItemAmount, 0, sizeof(MONEY_AMOUNT) );
		m_preItemName[0] = 0;
	}
	else
	{
		strcpy( m_preItemName, name );
		memcpy( &m_preItemAmount, &amount, sizeof(MONEY_AMOUNT) );

		MONEY_AMOUNT unitPrice;
		unitPrice.dollar = 0;
		unitPrice.cent = 0;

		if( ! m_transactionInformed )
		{
			m_receiver->setTransaction(); 

			m_transactionInformed = true;
		}

		m_receiver->setItem( name, unitPrice, 0, amount );   
	}

	return true;
}

bool CRubyCR0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Sub Total         7.00

	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setSubtotal( amount ); 

	m_transactionInformed = false;

	return true;
}

bool CRubyCR0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Tax         0.26

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

bool CRubyCR0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL         5.82!

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

bool CRubyCR0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"Cash",		CASH,		// Cash  $     10.00
		"Credit",	CREDIT,		// Credit  $     6.35
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

			PAYMENT_TYPE type = paymentType[i].type;
			
			// RubyCR0S may send a payment item two times. It's too bad !
			// I must discard the second one.
			if( m_prePaymentType == type && 
				memcmp( &m_prePaymentAmount, &amount, sizeof(MONEY_AMOUNT) ) == 0 )
			{
				memset( &m_prePaymentAmount, 0, sizeof(MONEY_AMOUNT) );
				m_prePaymentType = CASH;
			}
			else
			{
				m_prePaymentType = type;
				memcpy( &m_prePaymentAmount, &amount, sizeof(MONEY_AMOUNT) );
				
				m_receiver->setPaymentType( type, amount );  
			}

			return true;
		}
	}

	return false;
}

bool CRubyCR0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Change  $     -0.62

	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setChange( amount ); 

	return true;
}
