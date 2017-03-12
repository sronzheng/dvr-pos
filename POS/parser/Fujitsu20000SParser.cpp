// Fujitsu20000SParser.cpp: implementation of the CFujitsu20000SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fujitsu20000SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFujitsu20000SParser::CFujitsu20000SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( FUJITSU20000S_STATE_BEGINNING ),
   m_transactionInformed( false )
{

}

CFujitsu20000SParser::~CFujitsu20000SParser()
{

}

void CFujitsu20000SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case FUJITSU20000S_STATE_BEGINNING:
		if( setItem( p ) )
			m_state = FUJITSU20000S_STATE_SALE_ITEM_GOTTEN;

		break;

	case FUJITSU20000S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "BALANCE DUE" ) != NULL )
			m_state = FUJITSU20000S_STATE_BALANCE_DUE_GOTTEN;
		else if( strstr( p, "* VOID TRANSACTION *" ) != NULL )
		{
			if( setTransactionVoided( p ) )
				m_state = FUJITSU20000S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state.
		}

		break;
	
	case FUJITSU20000S_STATE_BALANCE_DUE_GOTTEN:
		if( setPaymentType( p ) )
			m_state = FUJITSU20000S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case FUJITSU20000S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL )
		{
			if( setChange( p ) )
				m_state = FUJITSU20000S_STATE_CHANGE_GOTTEN;
		}

		break;

	case FUJITSU20000S_STATE_CHANGE_GOTTEN:
		if( strstr( p, "SUB TOTAL" ) != NULL )
		{
			if( setSubtotal( p ) )
				m_state = FUJITSU20000S_STATE_SUBTOTAL_GOTTEN;
		}

		break;

	case FUJITSU20000S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "TOTAL TAX" ) != NULL )
		{
			if( setTax( p ) )
				m_state = FUJITSU20000S_STATE_TAX_GOTTEN;
		}
	
	case FUJITSU20000S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL )
		{
			if( setTotal( p ) )
				m_state = FUJITSU20000S_STATE_TOTAL_GOTTEN;
		}

		break;

	case FUJITSU20000S_STATE_TOTAL_GOTTEN:
	case FUJITSU20000S_STATE_TRANSACTION_VOIDED_GOTTEN:
		if( setTime( p ) )
			m_state = FUJITSU20000S_STATE_DATE_TIME_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

bool CFujitsu20000SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// C0202    #0039    16:17:42     4OCT2005 

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

	// look for day
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	p = pSlash + 1;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	int day = atoi( p );

	while( *p >= '0' && *p <= '9' )
		p++;

	// look for year
	int year = atoi( p+3 );

	// look for month
	*(p+3) = 0;

	int month = getMonth( p );

	m_receiver->setTime( year, month, day, hour, minute ); 

	m_transactionInformed = false;

	return true;
}

bool CFujitsu20000SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	//    MEAT                 $0.10   F

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

	// look for item name
	p = pStart;
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

bool CFujitsu20000SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	//  SUB TOTAL                  $0.50  

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

bool CFujitsu20000SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL TAX             $0.00    

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

bool CFujitsu20000SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL                      $0.50  
           
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

bool CFujitsu20000SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,		// CASH                       $1.00
	};

	// look for amount. 
	// if payment type matched, handle it, and stop search routine
	for ( int i = 0; i < sizeof( paymentType ) / sizeof( paymentType[0] ); i++ )
	{	
		char *pStr = strstr( p, paymentType[i].key );

		if ( pStr != NULL )
		{
			p = nextDigit( pStr );
			if ( p == NULL ) 
				return false;

			MONEY_AMOUNT amount;
			if( getMoneyAmount( p, amount ) == NULL )
				return false;

			m_receiver->setPaymentType( paymentType[i].type, amount);  
	
			return true;
		}
	}

	return false;
}

bool CFujitsu20000SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE                     $0.50

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

bool CFujitsu20000SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	//  * VOID TRANSACTION *

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}

