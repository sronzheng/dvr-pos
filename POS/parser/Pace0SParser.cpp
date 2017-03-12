// Pace0SParser.cpp: implementation of the CPace0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pace0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPace0SParser::CPace0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( PACE0S_STATE_BEGINNING ),
   m_preItemGotten( false ),
   m_transactionInformed( false )
{
	m_itemName[0] = 0;
	memset( &m_itemPrice, 0, sizeof(MONEY_AMOUNT) );
}

CPace0SParser::~CPace0SParser()
{

}

void CPace0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case PACE0S_STATE_BEGINNING:
		if( strstr( p, "Welcome to" ) != NULL ) 
			m_state = PACE0S_STATE_WELCOME_GOTTEN;

		break;

	case PACE0S_STATE_WELCOME_GOTTEN:
		if( strstr( p, "VOID TRANSACTION" ) != NULL )
		{
			if( setTransactionVoided( buf ) )
				m_state = PACE0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else if( strstr( p, "RESET TRANSACTION" ) != NULL )
			m_state = PACE0S_STATE_TRANSACTION_RESETED_GOTTEN;
		else if( strstr( p, "!#" ) == NULL ) 
			m_state = PACE0S_STATE_HEAD_PASSED;

		break;

	case PACE0S_STATE_HEAD_PASSED:
		if( setItem( p ) )
			m_state = PACE0S_STATE_SALE_ITEM_GOTTEN;

		break;

	case PACE0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "@" ) != NULL ) 
		{
			if( setItemMore( p ) )
				m_state = PACE0S_STATE_SALE_ITEM_MORE_GOTTEN;
		}
		else if( strstr( p, "SUBTOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = PACE0S_STATE_SUBTOTAL_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state.
		}

		break;

	case PACE0S_STATE_SALE_ITEM_MORE_GOTTEN:
		if( strstr( p, "SUBTOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = PACE0S_STATE_SUBTOTAL_GOTTEN;
		}
		else
		{
			if( setItem( p ) )
				m_state = PACE0S_STATE_SALE_ITEM_GOTTEN;
		}


		break;

	case PACE0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "TAX01" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = PACE0S_STATE_TAX_GOTTEN;
		}

		break;

	case PACE0S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = PACE0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case PACE0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = PACE0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case PACE0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CAS " ) != NULL ) 
		{
			if( setTime( p ) )
				m_state = PACE0S_STATE_DATE_TIME_GOTTEN;
		}
		else
		{	
			if( setPaymentType( p ) )
				m_state = PACE0S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case PACE0S_STATE_TRANSACTION_VOIDED_GOTTEN:
		if( strstr( p, "CAS " ) != NULL ) 
		{
			if( setTime( p ) )
				m_state = PACE0S_STATE_DATE_TIME_GOTTEN;
		}

		break;

	default:
		//never go here.
		break;
	}
}

bool CPace0SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// CAS  12/13/05   11:15AM #00064323 31

	// look for month
	p = nextDigit( p );

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
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int year = atoi( p ) + 2000;

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

	char *pMinute = p;

	// check whether it is a morning time, or afternoon time.
	while( *p >= '0' && *p <= '9' ) 
		p++;

	bool morning;
	if( strncmp( p, "AM", 2) == 0 )
		morning = true;
	else if( strncmp( p, "PM", 2) == 0 )
		morning = false;
	else
		return false;

	if( !morning )
		hour += 12;

	*p = 0;
	int minute = atoi( pMinute );

	m_receiver->setTime( year, month, day, hour, minute ); 

	m_transactionInformed = false;

	return true;
}

bool CPace0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// \xa 13245      MISCELLANEOUS MERCH    49.36
	//              4 @     12.34
	
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
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	p = nextDigit( pStart );
	if( p == NULL )
		return false;

	p = strchr( p, ' ');
	if( p == NULL )
		return false;

	while( *p == ' ' )
		p++;

	if( ! m_transactionInformed )
	{
		m_receiver->setTransaction(); 

		m_transactionInformed = true;
	}

	if( m_preItemGotten )
	{
		MONEY_AMOUNT unitPrice;
		unitPrice.dollar = abs( m_itemPrice.dollar );
		unitPrice.cent = abs( m_itemPrice.cent );
		
		m_receiver->setItem( m_itemName, unitPrice, 1, m_itemPrice ); 
	}

	// save sale item, and set flag to indicate the previous item valid.
	memcpy( &m_itemPrice, &amount, sizeof(MONEY_AMOUNT) );
	strcpy( m_itemName, p );

	m_preItemGotten = true;
	
	return true;
}

bool CPace0SParser::setItemMore( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// \xa 13245      MISCELLANEOUS MERCH    49.36
	//              4 @     12.34

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

	int quantity = atoi( p+1 );

	// look for item unit price
	p = nextDigit( pSlash+1 );
	if( p == NULL )
		return false;

	MONEY_AMOUNT unitPrice;
	if( getMoneyAmount( p, unitPrice ) == NULL )
		return false;

	m_receiver->setItem( m_itemName, unitPrice, quantity, m_itemPrice ); 
	
	// the previous sale item used, so clear the flag. 
	m_preItemGotten = false;

	return true;
}

bool CPace0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// SUBTOTAL               28.00

	// look for total
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

	if( m_preItemGotten )
	{
		m_receiver->setItem( m_itemName, m_itemPrice, 1, m_itemPrice ); 

		m_preItemGotten = false;
	}


	m_receiver->setSubtotal( amount ); 

	return true;
}

bool CPace0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TAX01                   4.25

	// EXMAPLE 2:
	// TAX01                    .27

	// EXMAPLE 2:
	// TAX01                  -2.58

	p = strstr( p, "TAX01" );
	if( p == NULL )
		return false;

	p = p + 5;

	// look for tax
	p = nextDigit( p );
	if( p == NULL )
		return false;

	if( *(p-1) == '.' )
		p--;

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

bool CPace0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL                -35.91

	// EXMAPLE 1:
	// TOTAL                 59.14
           
	// look for total
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

bool CPace0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,				// CASH          6.35
		"GIFT TEND",	GIFT_CERTIFICATE,	// GIFT TEND     5.82
		"CHECK",		CHECK,				// CHECK         5.82
		"BANK CARD",	ATM,				// BANK CARD     59.14
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

			if( *(p-1) == '-' )
			{
				amount.dollar	= -amount.dollar;
				amount.cent		= -amount.cent;
			}
			
			m_receiver->setPaymentType( paymentType[i].type, amount );  

			return true;
		}
	}

	return false;
}

bool CPace0SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	// !#1  VOID TRANSACTION  

	m_receiver->setTransaction(); 

	m_transactionInformed = true;
	
	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}