// DresserWayne0SParser.cpp: implementation of the CDresserWayne0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DresserWayne0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDresserWayne0SParser::CDresserWayne0SParser( CReceiver *receiver )
 : CParser( receiver ),
    m_state( DRESSERWAYNE0S_STATE_BEGINNING ),
	m_itemHeadGotten( false ),
	m_transactionInformed( false )
{
}

CDresserWayne0SParser::~CDresserWayne0SParser()
{

}

void CDresserWayne0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case DRESSERWAYNE0S_STATE_BEGINNING:
		if( strstr( p, "Safe Drop" ) != NULL ) 
			m_state = DRESSERWAYNE0S_STATE_SAFE_DROP_GOTTEN;
		else if( strstr( p, "PAID IN" ) != NULL ) 
			m_state = DRESSERWAYNE0S_STATE_PAID_IN_GOTTEN;
		else if( strstr( p, " @ " ) != NULL ) 
		{
			if( setItemHead( p ) )
				m_state = DRESSERWAYNE0S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}
		else
		{
			if( setItem( p ) )
				m_state = DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case DRESSERWAYNE0S_STATE_SALE_ITEM_HEAD_GOTTEN:
		if( setItem( p ) )
			m_state = DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN;

		break;

	case DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, " @ " ) != NULL ) 
		{
			if( setItemHead( p ) )
				m_state = DRESSERWAYNE0S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}
		else if( strstr( p, "ITEM CORRECT" ) != NULL ) 
		{
			setItemVoided( p );

			// keep state.
		}
		else if( strstr( p, "SUBTOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = DRESSERWAYNE0S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, "** CANCELLED **" ) != NULL ) 
		{
			if( setTransactionVoided( p ) )
				m_state = DRESSERWAYNE0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else
		{
			setItem( p );

			//keep state
		}

		break;

	case DRESSERWAYNE0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = DRESSERWAYNE0S_STATE_CHANGE_GOTTEN;
		}

		break;

	case DRESSERWAYNE0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "SALES TAX" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = DRESSERWAYNE0S_STATE_TAX_GOTTEN;
		}

		break;

	case DRESSERWAYNE0S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = DRESSERWAYNE0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case DRESSERWAYNE0S_STATE_TOTAL_GOTTEN:
		if( strstr( p, "** CANCELLED **" ) != NULL ) 
		{
			if( setTransactionVoided( p ) )
				m_state = DRESSERWAYNE0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = DRESSERWAYNE0S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case DRESSERWAYNE0S_STATE_CHANGE_GOTTEN:
	case DRESSERWAYNE0S_STATE_TRANSACTION_VOIDED_GOTTEN:
		if( setTime( p ) )
			m_state = DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN;
		break;

	case DRESSERWAYNE0S_STATE_SAFE_DROP_GOTTEN:
	case DRESSERWAYNE0S_STATE_PAID_IN_GOTTEN:
		if( setTime( p, false ) )
			m_state = DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN;
		break;

	default:
		//never go here.
		break;
	}
}

bool CDresserWayne0SParser::setTime( char *buffer, bool inform  )
{
	char *p = buffer;

	// EXAMPLE:
	// 0001e12s01t1 12:40:20pm Mon 13 Oct 03 

	// look for hour
	char *pSlash = strchr( p, ':' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	p = strrchr( p, ' ');
	if( p == NULL )
		return false;

	int hour = atoi( p+1 );

	p = pSlash + 1;

	// look for minute
	pSlash = strchr( p, ':' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int minute = atoi( p );

	p = pSlash + 1;

	// skip second and weekday
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	p = pSlash+1;

	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	p = pSlash+1;

	// look for day
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int day = atoi( p );

	p = pSlash + 1;

	// look for month
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int month = getMonth( strupr(p) );

	p = pSlash + 1;

	// look for year
	int year = atoi( p ) + 2000;

	m_transactionInformed = false;

	if( inform )
		m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CDresserWayne0SParser::setItemHead( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// S2\x1B 4                       2    @   4.99
	// S2\x1B 4 #08380404721    Cooks Brut C   5.95 T

	char *pStart = p;

	p = strstr( p, "S2\x1B" );
	if( p == NULL )
		return false;

	char *pSlash = strchr( p, '@' );
	if( pSlash == NULL )
		return false;

	// look for quantity
	p = preDigit( pSlash, p );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	m_itemQuantity = atoi( p+1 );

	// look for item unit price
	p = nextDigit( pSlash+1 );
	if( p == NULL )
		return false;

	if( getMoneyAmount( p, m_itemUnitPrice ) == NULL )
		return false;

	m_itemHeadGotten = true;

	return true;
}

bool CDresserWayne0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// S2\x1B 4                       2    @   4.99
	// S2\x1B 4 #08380404721    Cooks Brut C   5.95 T
	
	// EXAMPLE 2: 
	// S2\x1B 4 #08380404721    Cooks Brut C   5.95 T

	// EXAMPLE 3: 
	// S2\x1B 4                 GROCERY-TAX    0.79 T

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

	// discard the space characters at tail. 
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	p = strstr( pStart, "S2\x1B" );
	if( p == NULL )
		return false;

	//look for item name
	p = strchr( p, ' ' );
	if( p == NULL )
		return false;

	// skip a digit, if it starts with #.
	char* pSlash = strchr( pStart, '#' );
	if( pSlash != NULL )
	{
		p = strchr( pSlash, ' ');
		if( p == NULL )
			return false;
	}

	while( *p == ' ' )
		p++;

	if( ! m_transactionInformed )
	{
		m_receiver->setTransaction();

		m_transactionInformed = true;
	}

	if( m_itemHeadGotten )
	{
		m_receiver->setItem( p, m_itemUnitPrice, m_itemQuantity, amount ); 

		m_itemHeadGotten = false;
	}
	else
	{
		MONEY_AMOUNT unitPrice;
		unitPrice.dollar = abs( amount.dollar );
		unitPrice.cent = abs( amount.cent );

		m_receiver->setItem( p, unitPrice, 1, amount ); 
	}
	
	return true;
}

bool CDresserWayne0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// ITEMS:  3        SUBTOTAL       6.08  

	// look for total
	p = strstr( p, "SUBTOTAL" );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setSubtotal( amount ); 

	return true;
}

bool CDresserWayne0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// 4                 SALES TAX      2.17

	// look for tax
	p = strstr( p, "SALES TAX" );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setTax( amount ); 

	return true;
}

bool CDresserWayne0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL      \x1B 4         5.85  

	char *pStart = p;
	p = p + strlen( p );

	// look for total
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p+1, amount ) == NULL )
		return false;

	m_receiver->setTotal( amount ); 

	return true;
}

bool CDresserWayne0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"Indoor Cash",			CASH,		// Indoor Cash  100.00
		"Indoor Debit",			ATM,		// Indoor Debit  76.83
	};

	// look for amount. 
	// if payment type matched, handle it, and stop search routine
	for ( int i = 0; i < sizeof( paymentType ) / sizeof( paymentType[0] ); i++ )
	{	
		char *pStr = strstr( p, paymentType[i].key );

		if ( pStr != NULL )
		{
			// look for amount
			p = nextDigit( pStr );
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

bool CDresserWayne0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE                          7.53
	// 3            CHANGE DUE \x1B 4  7.53  

	char *pStart = p;
	p = p + strlen( p );

	// look for change
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p+1, amount ) == NULL )
		return false;

	m_receiver->setChange( amount ); 

	return true;
}

bool CDresserWayne0SParser::setItemVoided( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// ITEM CORRECT   4.99- 

	char *pStart = p;

	p = p + strlen( p );

	// look for amount
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0; 

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	char *pSlash = getMoneyAmount( p+1, amount);
	if( ! pSlash )
		return false;

	m_receiver->setItemVoided( amount ); 

	return true;
}

bool CDresserWayne0SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	//  *** CANCELLED *** 

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}
