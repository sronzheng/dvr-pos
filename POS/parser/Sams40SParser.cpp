// Sams40SParser.cpp: implementation of the CSams40SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sams40SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSams40SParser::CSams40SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( SAMS40S_STATE_BEGINNING )
{

}

CSams40SParser::~CSams40SParser()
{
}

void CSams40SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case SAMS40S_STATE_BEGINNING:
		if( setTime( p ) )
			m_state = SAMS40S_STATE_DATE_TIME_GOTTEN;

		break;

	case SAMS40S_STATE_DATE_TIME_GOTTEN:
		if( setItem( p ) )
			m_state = SAMS40S_STATE_SALE_ITEM_GOTTEN;

		break;

	case SAMS40S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = SAMS40S_STATE_TOTAL_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state.
		}

		break;

	case SAMS40S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = SAMS40S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case SAMS40S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = SAMS40S_STATE_CHANGE_GOTTEN;
		}

		break;

	default:
		//never go here.
		break;
	}
}

bool CSams40SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// DATE 03/18/2005 FRI   TIME 12:43

	p = nextDigit( p );
	if( p == NULL )
		return false;

	// look for month
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
	int year = atoi( p );
	
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
	
	int minute = atoi( p );

	m_receiver->setTransaction();

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CSams40SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// PEPSI                      $2.58 

	char *pStart = buffer;

	p = strchr( pStart, '$');
	if( p == NULL )
		return false;

	MONEY_AMOUNT price;
	if( getMoneyAmount( p+1, price ) == NULL )
		return false;

	// discard space characters at tail, preparing to look for item name
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	MONEY_AMOUNT unitPrice;
	unitPrice.dollar = 0;
	unitPrice.cent = 0;

	m_receiver->setItem( pStart, unitPrice, 0, price );   

	return true;
}

bool CSams40SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL                     $16.95
           
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

bool CSams40SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,	// CASH                      $20.00
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

bool CSams40SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE                     $3.05

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
