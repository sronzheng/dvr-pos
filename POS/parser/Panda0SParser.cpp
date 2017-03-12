// Panda0SParser.cpp: implementation of the CPanda0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Panda0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanda0SParser::CPanda0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( PANDA0S_STATE_BEGINNING ),
   m_transactionInformed( false )
{
}

CPanda0SParser::~CPanda0SParser()
{
}

void CPanda0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case PANDA0S_STATE_BEGINNING:
		if( strstr( p, "**REGISTER**" ) != NULL ) 
			m_state = PANDA0S_STATE_REGISTER_GOTTEN;
		else
		{
			if( setItem( p ) )
				m_state = PANDA0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case PANDA0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "TAX" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = PANDA0S_STATE_TAX_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state
		}

		break;

	case PANDA0S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = PANDA0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case PANDA0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = PANDA0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case PANDA0S_STATE_PAYMENT_TYPE_GOTTEN:

		if( strstr( p, "CHNG" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = PANDA0S_STATE_CHANGE_GOTTEN;
		}
		else
		{	
			if( setPaymentType( p ) )
				m_state = PANDA0S_STATE_PAYMENT_TYPE_GOTTEN;
			else if( setTime( p ) )
				m_state = PANDA0S_STATE_DATE_TIME_GOTTEN;
		}

		break;

	case PANDA0S_STATE_CHANGE_GOTTEN:
		if( setTime( p ) )
			m_state = PANDA0S_STATE_DATE_TIME_GOTTEN;

		break;

	case PANDA0S_STATE_REGISTER_GOTTEN:
		if( setTime( p, false ) )
			m_state = PANDA0S_STATE_DATE_TIME_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CPanda0SParser::setTime( char *buffer, bool inform )
{
	char *p = buffer;

	// EXAMPLE:
	// v 129 10:28 #08 MAY.27'05  REG0001

	char *pStart = buffer;

	char *pSharp = strchr( p, '#' );
	if( pSharp == NULL )
		return false;

	// look for minute
	p = preDigit( pSharp, p );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ':');
	if( p == NULL )
		return false;

	int minute = atoi( p+1 );

	// look for hour
	*p = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	int	hour = atoi( p+1 );

	// look for the start position of month
	p = pSharp + 1;

	while( *p == ' ' || 
		   (*p >= '0' && *p <= '9') )
		p++;

	// look for month
	char *pSlash = strchr( p, '.' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	int month = getMonth( p );
	if( month < 0 )
		return false;

	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, '\'' );
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

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CPanda0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// \x1B v  1 S DOMSTCBEER              8.99
	
	// EXAMPLE 2: sale item
	// \x1B v  1 S DOMSTCBEER              .99

	char *pStart = buffer;

	p = p + strlen( p );

	// look for price
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT price;
	if( getMoneyAmount( p+1, price ) == NULL )
		return false;

	// discard space characters at tail, preparing to look for item name
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	// skip prefix characters in front, preparing to look for item name
	p = strrchr( pStart, '\x1B');
	if( p == NULL )
		return false;

	p = nextDigit( p ); 
	if( p == NULL )
		return false;

	// look for item name
	p = strchr( p, ' ');
	if( p == NULL )
		return false;

	while( *p == ' ' )
		p++;

	MONEY_AMOUNT unitPrice;
	unitPrice.dollar = 0;
	unitPrice.cent = 0;

	if( ! m_transactionInformed )
	{
		m_receiver->setTransaction(); 

		m_transactionInformed = true;
	}

	m_receiver->setItem( p, unitPrice, 0, price );   

	return true;
}

bool CPanda0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TAX               1.51

	// EXMAPLE 1:
	// TAX                .51

	// look for tax
	p = nextDigit( p );
	if( p == NULL )
		return false;

	if( *(p-1) == '.' )
		p--;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_transactionInformed = false;

	m_receiver->setTax( amount ); 

	return true;
}

bool CPanda0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTL          ! 5.82!

	// EXMAPLE 1:
	// TOTL          ! .82!

	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	if( *(p-1) == '.' )
		p--;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setTotal( amount ); 

	return true;
}

bool CPanda0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",		CASH,		// CASH             41.11
	};

	// look for amount. 
	// if payment type matched, handle it, and stop search routine
	for ( int i = 0; i < sizeof( paymentType ) / sizeof( paymentType[0] ); i++ )
	{	
		char *pStr = strstr( p, paymentType[i].key );

		if ( pStr != NULL )
		{
			p = nextDigit( p );
			if( p == NULL )
				return false;
			
			if( *(p-1) == '.' )
				p--;
			
			MONEY_AMOUNT amount;
			if( getMoneyAmount( p, amount ) == NULL )
				return false;
			
			m_receiver->setPaymentType( paymentType[i].type, amount );  

			return true;
		}
	}

	return false;
}

bool CPanda0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHNG             20.14

	// look for change
	p = nextDigit( p );
	if( p == NULL )
		return false;

	if( *(p-1) == '.' )
		p--;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setChange( amount ); 

	return true;
}
