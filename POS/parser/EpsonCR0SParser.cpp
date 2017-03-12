// EpsonCR0SParser.cpp: implementation of the CEpsonCR0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EpsonCR0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEpsonCR0SParser::CEpsonCR0SParser( CReceiver *receiver )
 : CParser( receiver ),
    m_state( EPSONCR0S_STATE_BEGINNING ),
   m_itemHeadGotten( false )
{

}

CEpsonCR0SParser::~CEpsonCR0SParser()
{

}

void CEpsonCR0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case EPSONCR0S_STATE_BEGINNING:
		if( strstr( p, "AM") != NULL || strstr( p, "PM") != NULL )
		{
			if( setTime( p ) )
				m_state = EPSONCR0S_STATE_DATE_TIME_GOTTEN;
		}

		break;

	case EPSONCR0S_STATE_DATE_TIME_GOTTEN:
		if( strstr( p, "NO SALE" ) != NULL ) 
			m_state = EPSONCR0S_STATE_NO_SALE_GOTTEN;
		else if( strstr( p, " @ " ) != NULL ) 
		{
			if( setItemHead( p ) )
				m_state = EPSONCR0S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}
		else
		{
			if( setItem( p ) )
				m_state = EPSONCR0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case EPSONCR0S_STATE_SALE_ITEM_HEAD_GOTTEN:
		if( setItem( p ) )
			m_state = EPSONCR0S_STATE_SALE_ITEM_GOTTEN;

		break;

	case EPSONCR0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, " @ " ) != NULL ) 
		{
			if( setItemHead( p ) )
				m_state = EPSONCR0S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}
		else if( strstr( p, "REMOVED" ) != NULL ) 
		{
			if( setItemVoided( p ) )
				m_state = EPSONCR0S_STATE_SALE_ITEM_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = EPSONCR0S_STATE_PAYMENT_TYPE_GOTTEN;
			else if( setItem( p ) )
				m_state = EPSONCR0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case EPSONCR0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "SUBTOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = EPSONCR0S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, "Please Come Again" ) != NULL ) 
			m_state = EPSONCR0S_STATE_THANK_GOTTEN;

		break;

	case EPSONCR0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "PST " ) != NULL || strstr( p, "GST " ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = EPSONCR0S_STATE_TAX_GOTTEN;
		}

		break;

	case EPSONCR0S_STATE_TAX_GOTTEN:
		if( strstr( p, "PST " ) != NULL || strstr( p, "GST " ) != NULL ) 
		{
			setTax( p );

			// keep state
		}
		else if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = EPSONCR0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case EPSONCR0S_STATE_TOTAL_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = EPSONCR0S_STATE_CHANGE_GOTTEN;
		}
	
		break;

	case EPSONCR0S_STATE_CHANGE_GOTTEN:
	case EPSONCR0S_STATE_NO_SALE_GOTTEN:
		if( strstr( p, "Please Come Again" ) != NULL ) 
			m_state = EPSONCR0S_STATE_THANK_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CEpsonCR0SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// = \x1 12/11/2003         10:32 AM         000190

	// look for month
	char *pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	p = pSlash-2;
	int month = atoi( p );

	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int day = atoi( p );

	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int year = atoi( p );

	p = pSlash + 1;

	// look for hour
	p = nextDigit( p );
	if( p == NULL )
		return false;

	pSlash = strchr( p, ':' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int hour = atoi( p );

	p = pSlash + 1;

	// look for minute
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int minute = atoi( p );

	p = pSlash + 1;

	// check if it is morning or afternoon.
	bool morning;
	if( strncmp( p, "AM", 2) == 0 )
		morning = true;
	else if( strncmp( p, "PM", 2) == 0 )
		morning = false;
	else
		return false;

	if( !morning )
		hour += 12;

	m_receiver->setTransaction(); 

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CEpsonCR0SParser::setItemHead( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// = \x1             5  @ 1.1900                   
	// = \x1 06490000057   WRIG ALPINE CH EACH     5.95

	char *pStart = p;

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

bool CEpsonCR0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// = \x1             5  @ 1.1900                   
	// = \x1 06490000057   WRIG ALPINE CH EACH     5.95
	
	// EXAMPLE 2: 
	// = \x1 06490000057   WRIG ALPINE CH EACH     5.95

	// EXAMPLE 3: 
	// = \x1 Coupon Store                         -0.29

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

	if( *(p+1) == '-' )
	{
		amount.dollar = -amount.dollar; 
		amount.cent = -amount.cent; 
	}

	//look for item name

	// discard the space characters at tail. 
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	p = strchr( pStart, '\x1' );
	if( p == NULL )
		return false;

	// skip a digit, if it starts with digit.
	char* pSlash = nextDigit( p );
	if( pSlash != NULL )
	{
		p = strchr( pSlash, ' ');
		if( p == NULL )
			return false;
		
		while( *(p+1) == ' ' )
			p++;
	}

	if( m_itemHeadGotten )
	{
		m_receiver->setItem( p+1, m_itemUnitPrice, m_itemQuantity, amount ); 

		m_itemHeadGotten = false;
	}
	else
	{
		MONEY_AMOUNT unitPrice;
		unitPrice.dollar = abs( amount.dollar );
		unitPrice.cent = abs( amount.cent );

		m_receiver->setItem( p+1, unitPrice, 1, amount ); 
	}
	
	return true;
}

bool CEpsonCR0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// SUBTOTAL                  -1.45

	// EXMAPLE 2:
	// SUBTOTAL                   5.08

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

	// look for sign of tax amount.
	if( *(p-1) == '-' )
	{
		amount.dollar	= -amount.dollar;
		amount.cent		= -amount.cent;
	}

	m_receiver->setSubtotal( amount ); 

	return true;
}

bool CEpsonCR0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// PST                                   2.17

	// EXMAPLE 2:
	// GST                                   -2.17

	// look for tax
	p = strstr( p, "ST " );
	if( p == NULL )
		return false;

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

bool CEpsonCR0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL                                -5.82
           
	// EXMAPLE 2:
	// TOTAL                                5.82

	// look for total

	p = strstr( p, "TOTAL" );
	if( p == NULL )
		return false;

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

	m_receiver->setTotal( amount ); 

	return true;
}

bool CEpsonCR0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	// I don't know what "American Express" is, so don't handle it.
	if( strstr( p, "American Express" ) != NULL ) 
		return true;

	// keep "Visa Cash" in the front of "Cash", avoiding to be taken as CASH item.
	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"Master Card",		CREDIT,				// Master Card          5.82
		"Visa Cash",		CREDIT,				// Visa Cash            5.82
		"Cash",				CASH,				// Cash				    6.35
		"Gift Certificates",GIFT_CERTIFICATE,	// Gift Certificates    5.82
		"Debit",			ATM,				// Debit                5.82
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

bool CEpsonCR0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE                          7.53

	// look for change
	p = strstr( p, "CHANGE" );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setChange( amount ); 

	return true;
}

bool CEpsonCR0SParser::setItemVoided( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// REMOVED       Coupon Store            0.29

	// look for amount
	p = strstr( p, "REMOVED" );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setItemVoided( amount ); 

	return true;
}
