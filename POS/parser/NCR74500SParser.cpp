// NCR74500SParser.cpp: implementation of the CNCR74500SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCR74500SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCR74500SParser::CNCR74500SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( NCR74500S_STATE_BEGINNING )
{
}

CNCR74500SParser::~CNCR74500SParser()
{

}

void CNCR74500SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case NCR74500S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
			{
				m_state = NCR74500S_STATE_CHANGE_GOTTEN;
				break;
			}
		}
		else
		{	
			if( setPaymentType( p ) )
			{
				m_state = NCR74500S_STATE_PAYMENT_TYPE_GOTTEN;
				break;
			}
		}

		// go through.
		// if it is not a change item or a payment item, try to handle it as a time item.

		//break;

	case NCR74500S_STATE_BEGINNING:
		if( setTime( p ) )
			m_state = NCR74500S_STATE_DATE_TIME_GOTTEN;

		break;

	case NCR74500S_STATE_DATE_TIME_GOTTEN:
		if( strstr( p, "THANKS FOR SHOPPING" ) != NULL )
			m_state = NCR74500S_STATE_WELCOME_GOTTEN;

		break;

	case NCR74500S_STATE_WELCOME_GOTTEN:
		if( strstr( p, "NO SALE" ) != NULL )
			m_state = NCR74500S_STATE_NO_SALE_GOTTEN;
		else if( strstr( p, "SAFE DROP" ) != NULL )
			m_state = NCR74500S_STATE_SAFE_DROP_GOTTEN;
		else
		{
			if( setItem( p ) )
				m_state = NCR74500S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case NCR74500S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "SUBTOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = NCR74500S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, "<<< TRANSACTION ABORTED >>>" ) != NULL ) 
		{
			if( setTransactionVoided( p ) )
				m_state = NCR74500S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state.
		}

		break;

	case NCR74500S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = NCR74500S_STATE_TOTAL_GOTTEN;
		}
		else if( strstr( p, "SALES TAX ON" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = NCR74500S_STATE_TAX_GOTTEN;
		}

		break;

	case NCR74500S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTAL DUE" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = NCR74500S_STATE_TOTAL_GOTTEN;
		}

		break;

	case NCR74500S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = NCR74500S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}

}

bool CNCR74500SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// T#01 OP06 TRN2823 03/03/19 01:15 pm

	// look for year
	char *pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	p = pSlash - 2;
	int year = atoi( p ) + 2000;

	p = pSlash + 1;

	// look for month
	pSlash = strchr( p, '/' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int month = atoi( p );

	p = pSlash + 1;

	// look for day
	pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;
	int day = atoi( p );

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
	if( strncmp( p, "am", 2) == 0 )
		morning = true;
	else if( strncmp( p, "pm", 2) == 0 )
		morning = false;
	else
		return false;

	if( !morning )
		hour += 12;

	m_receiver->setTransaction(); 

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CNCR74500SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// 1  Newp Bx Kg                     2.90T

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

bool CNCR74500SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// SUBTOTAL             2.38

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

bool CNCR74500SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// SALES TAX ON 2.90                  0.21

	// skip a digit.
	p = nextDigit( p );
	if( p == NULL )
		return false;

	p = strchr( p, ' ' );
	if( p == NULL ) 
		return false;

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

bool CNCR74500SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL DUE             3.11 
           
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

bool CNCR74500SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,	// CASH          3.11
		"SV Card",		CREDIT,	// SV Card       1.81
		"DEBIT",		ATM,	// DEBIT        20.71
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

bool CNCR74500SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE             0.62

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

bool CNCR74500SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	// **TRANSACTION VOIDED**

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}
