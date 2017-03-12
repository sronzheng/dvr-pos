// Casio6100A0SParser.cpp: implementation of the CCasio6100A0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Casio6100A0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCasio6100A0SParser::CCasio6100A0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( CASIO6100A0S_STATE_BEGINNING )
{
}

CCasio6100A0SParser::~CCasio6100A0SParser()
{

}

void CCasio6100A0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, " CG " ) != NULL ) 
		{
			if( setChange( p ) )
			{
				m_state = CASIO6100A0S_STATE_CHANGE_GOTTEN;
				break;
			}
		}
		else
		{	
			if( setPaymentType( p ) )
			{
				m_state = CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN;
				break;
			}
		}

		// go through.
		// if it is not a change item or a payment item, try to handle it as a time item.

		//break;

	case CASIO6100A0S_STATE_BEGINNING:
		if( setTime( p ) )
			m_state = CASIO6100A0S_STATE_DATE_TIME_GOTTEN;

		break;

	case CASIO6100A0S_STATE_DATE_TIME_GOTTEN:
		if( setItem( p ) )
			m_state = CASIO6100A0S_STATE_SALE_ITEM_GOTTEN;

		break;

	case CASIO6100A0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, " TL " ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = CASIO6100A0S_STATE_TOTAL_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state.
		}

		break;

	case CASIO6100A0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CCasio6100A0SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// REG            17-05-2003(SAT)  13:01   

	p = nextDigit( p );
	if( p == NULL )
		return false;

	// look for day
	char *pSlash = strchr( p, '-' );
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
	pSlash = strchr( p, '(' );
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
	
	// look for minute
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL)
		return false;

	*pSlash = 0;
	int minute = atoi( p );

	m_receiver->setTransaction();

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CCasio6100A0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	//  1  DRINKS                   2.00   

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

	p = nextDigit( pStart ); 
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

	m_receiver->setItem( p, unitPrice, 0, price );   

	return true;
}

bool CCasio6100A0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TL               !! 8.70!
           
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

bool CCasio6100A0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,	// CASH              10.00 
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

bool CCasio6100A0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	//  CG                4.00   

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
