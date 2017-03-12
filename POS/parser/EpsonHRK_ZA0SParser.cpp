// EpsonHRK_ZA0SParser.cpp: implementation of the CEpsonHRK_ZA0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EpsonHRK_ZA0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEpsonHRK_ZA0SParser::CEpsonHRK_ZA0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( EPSONHRK_ZA0S_STATE_BEGINNING )
{
}

CEpsonHRK_ZA0SParser::~CEpsonHRK_ZA0SParser()
{

}

void CEpsonHRK_ZA0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case EPSONHRK_ZA0S_STATE_BEGINNING:
		if( strstr( p, "WELCOME TO" ) != NULL ) 
		{
			if( setWelcome( p ) )
				m_state = EPSONHRK_ZA0S_STATE_WELCOME_GOTTEN;
		}

		break;

	case EPSONHRK_ZA0S_STATE_WELCOME_GOTTEN:
		if( strstr( p, "TR VOID" ) != NULL )
		{
			if( setTransactionVoided( buf ) )
				m_state = EPSONHRK_ZA0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else if( strstr( p, "PAID OUT" ) != NULL )
				m_state = EPSONHRK_ZA0S_STATE_PAID_OUT_GOTTEN;
		else
		{
			if( setItem( p ) )
				m_state = EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "TOTAL" )	!= NULL && 
			strstr( p, "  TOTAL" )	== NULL ) 
		{
			if( setTotal( p ) )
				m_state = EPSONHRK_ZA0S_STATE_TOTAL_GOTTEN;
		}
		else
		{
			setItem( p );

			//keep state.
		}

		break;

	case EPSONHRK_ZA0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = EPSONHRK_ZA0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case EPSONHRK_ZA0S_STATE_PAID_OUT_GOTTEN:
		if( setTime( p, false ) )
			m_state = EPSONHRK_ZA0S_STATE_DATE_TIME_GOTTEN;

		break;

	case EPSONHRK_ZA0S_STATE_TRANSACTION_VOIDED_GOTTEN:
	case EPSONHRK_ZA0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( setTime( p ) )
			m_state = EPSONHRK_ZA0S_STATE_DATE_TIME_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CEpsonHRK_ZA0SParser::setWelcome( char *buffer )
{
	// just inform a transaction started.

	m_receiver->setTransaction();
	
	return true;
}

bool CEpsonHRK_ZA0SParser::setTime( char *buffer, bool inform )
{
	char *p = buffer;

	// EXAMPLE:
	// #01300 14:46 21-06-2004 004 CLK0626

	// look for the starting position of time.
	p = strchr( p, '#' );
	if( p == NULL )
		return false;

	p = strchr( p, ' ');
	if( p == NULL )
		return false;

	// look for hour
	p = nextDigit( p );
	if( p == NULL )
		return false;

	char *pSlash = strchr( p, ':' );
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

	// look for day
	pSlash = strchr( p, '-' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int day = atoi( p );

	p = pSlash + 1;

	// look for month
	pSlash = strchr( p, '-' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int month = atoi( p );

	p = pSlash + 1;

	// look for year
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	int year = atoi( p );

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CEpsonHRK_ZA0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	//  CASTLE DUMPIES 34 340    x12   8.99
	
	// EXAMPLE 2: sale item
	//  BLACK LABEL DUMPI 340           4.40

	// EXAMPLE 3: sale item
	//  CASTLE CANS 340   340          8.99-

	// EXAMPLE 4: item voided
	//  BLACK LABEL CANS  340    x12   43.25-VD

	char *pStart = p;

	p = p + strlen( p );

	// look for price
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT price;
	char *pSlash = getMoneyAmount( p+1, price);
	if( ! pSlash )
		return false;

	if( strcmp( pSlash, "-VD" ) == 0 )
	{
		// if it is a voided time, return now.
		// check if it is a voided time.
		m_receiver->setItemVoided( price ); 

		return true;
	}
	else if( *pSlash == '-' )
	{
		// check sign of sale item.
		price.dollar = -price.dollar; 
		price.cent = -price.cent; 
	}

	// skip one or two digit.
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	while( *p >= '0' && *p <= '9' )
		p--;

	if( *p == 'x' )
	{
		// more one digit need to be skipped.
		p = preDigit( p, pStart );
		*(p+1) = 0;
		
		p = strrchr( pStart, ' ' );
		if( p == NULL )
			return false;
	}
	else if( *p == ' ' )
	{
		// no more digit need to be skipped. do nothing. 
	}
	else
		return false;

	// look for item name.
	while ( *(p-1) == ' ' )
		p--;

	*p = 0;

	p = pStart;
	while( *p == ' ' )
		p++;

	MONEY_AMOUNT unitPrice;
	unitPrice.dollar = 0;
	unitPrice.cent = 0;

	m_receiver->setItem( p, unitPrice, 0, price );   

	return true;
}

bool CEpsonHRK_ZA0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL        R125.60

	// EXMAPLE 1:
	//  TOTAL       R5.82-
           
	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	p = getMoneyAmount( p, amount );
	if( p == NULL )
		return false;

	// look for sign of tax amount.
	if( *p == '-' )
	{
		amount.dollar	= -amount.dollar;
		amount.cent		= -amount.cent;
	}

	m_receiver->setTotal( amount ); 

	return true;
}

bool CEpsonHRK_ZA0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,				// CASH      4.40-
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
			p = getMoneyAmount( p, amount );
			if( p == NULL )
				return false;

			if( *p == '-' )
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

bool CEpsonHRK_ZA0SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	// TRANS 1300  TR VOID

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}
