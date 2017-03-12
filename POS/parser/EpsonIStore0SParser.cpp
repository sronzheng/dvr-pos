// EpsonIStore0SParser.cpp: implementation of the CEpsonIStore0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EpsonIStore0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEpsonIStore0SParser::CEpsonIStore0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( EPSONISTORE0S_STATE_BEGINNING )
{
	m_itemName[0] = 0;
}

CEpsonIStore0SParser::~CEpsonIStore0SParser()
{
}

void CEpsonIStore0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case EPSONISTORE0S_STATE_THANK_GOTTEN:
		if( strstr( p, "**COMPETITION ENTRY SLIP**" ) != NULL ) 
		{
			m_state = EPSONISTORE0S_STATE_COMPLETION_START_GOTTEN;

			break;
		}
		else
		{
			if( setPaymentType( p ) )
			{
				m_state = EPSONISTORE0S_STATE_PAYMENT_TYPE_GOTTEN;

				break;
			}
		}

		// fall through.
		// break;

	case EPSONISTORE0S_STATE_BEGINNING:
		if( strstr( p, "Date  :" ) != NULL ) 
		{
			if( setTime( p ) )
				m_state = EPSONISTORE0S_STATE_DATE_TIME_GOTTEN;
		}

		break;

	case EPSONISTORE0S_STATE_DATE_TIME_GOTTEN:
		if( strstr( p, "TRANSACTION VOID" ) != NULL ) 
		{
			if( setTransactionVoided( p ) )
				m_state = EPSONISTORE0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, "Date  :" ) != NULL ) 
		{
			if( setTime( p ) )
				m_state = EPSONISTORE0S_STATE_DATE_TIME_GOTTEN;
		}
		else
		{
			if( setItem( p ) != EPSONISTORE0S_NULL_ITEM  )
				m_state = EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, " @ " ) != NULL ) 
		{
			setItemMore( p );

			// keep state.
		}
		else if( strstr( p, "*REVERSE" ) != NULL ) 
		{
			setItemVoided( p );

			// keep state.
		}
		else if( strstr( p, "TRANSACTION VOID" ) != NULL ) 
		{
			if( setTransactionVoided( p ) )
				m_state = EPSONISTORE0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}
		else
		{
			setItem( p );

			// keep state.
		}

		break;

	case EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "TOTAL AMOUNT" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = EPSONISTORE0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case EPSONISTORE0S_STATE_TOTAL_GOTTEN:
		if( strstr( p, "** PLEASE COME AGAIN **" ) != NULL ) 
			m_state = EPSONISTORE0S_STATE_THANK_GOTTEN;
		else
		{
			if( setPaymentType( p ) )
				m_state = EPSONISTORE0S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case EPSONISTORE0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
				m_state = EPSONISTORE0S_STATE_CHANGE_GOTTEN;
		}

		break;

	case EPSONISTORE0S_STATE_CHANGE_GOTTEN:
		if( strstr( p, "** PLEASE COME AGAIN **" ) != NULL ) 
			m_state = EPSONISTORE0S_STATE_THANK_GOTTEN;

		break;

	case EPSONISTORE0S_STATE_COMPLETION_START_GOTTEN:
		if( strstr( p, "==========================" ) != NULL ) 
			m_state = EPSONISTORE0S_STATE_COMPLETION_END_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}

}

bool CEpsonIStore0SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// Date  : 09/08/04        Time : 11:52

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
	int minute = atoi( p );

	m_receiver->setTransaction();

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

EPSONISTORE0S_ITEM_TYPE CEpsonIStore0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// PUMP PARAFFIN 20LT                  9.99

	// EXAMPLE 2: sale item
	// PUMP PARAFFIN 20LT                  *.**

	// EXAMPLE 3: sale item
	// PUMP PARAFFIN 20LT
	// 62114         *  6    @    9.99=   59.94

	// EXAMPLE 4: sale item
	// PRIMUS SILENT BURNER 2.170
	// 62118         *  3    @   17.35=   52.05

	char *pStart = buffer;

	MONEY_AMOUNT price;

	p = strstr( p, "*.*" );
	if( p != NULL )
	{
		//
		// if "*.*" is included, it's a item with invisible price.
		//

		memset( &price, 0, sizeof(MONEY_AMOUNT) );

		*p = 0;

		p = strrchr( pStart, ' ' );
		if( p == NULL )
			return EPSONISTORE0S_NULL_ITEM;
	}
	else
	{
		//
		// try to find price, without changing data.
		// if not found,  take it as item head.
		//

		p = pStart + strlen( pStart );

		p = preDigit( p, pStart );
		if( p == NULL )
			goto item_head;

		while( *p >= '0' && *p <= '9'  )
			p--;

		if( *p != '.' )
			goto item_head;

		while( *(p-1) >= '0' && *(p-1) <= '9'  )
			p--;

		// thers are at lease two space characters between item name 
		// and price. 
		if( *(p-2) != ' ' )
			goto item_head;
		
		if( getMoneyAmount( p, price ) == NULL )
			return EPSONISTORE0S_NULL_ITEM;
		
		if( *(p-1) == '-' )
		{
			price.dollar = -price.dollar;
			price.cent = -price.cent;
		}
		
		p--;
	}

	// look for item name.
	// discard space characters at tail.
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	m_receiver->setItem( pStart, price, 1, price );   

	return EPSONISTORE0S_WHOLE_ITEM;

item_head:

	p = pStart + strlen( pStart );

	// look for item name
	// discard space characters at tail.
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	strcpy( m_itemName, pStart );

	return EPSONISTORE0S_HEAD_ITEM;
}

bool CEpsonIStore0SParser::setItemMore( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// PUMP PARAFFIN 20LT
	// 62114         *  6    @    9.99=   59.94

	// EXAMPLE 2: sale item
	// PUMP PARAFFIN 20LT
	// 62114         *  6    @    -9.99=   -59.94

	// EXAMPLE 3: sale item
	// PUMP PARAFFIN 20LT
	// 62114         ****    @    9.99=   **.**

	char *pStart = p;

	char *pSlash = strchr( p, '@' );
	if( pSlash == NULL )
		return false;

	*pSlash = 0;

	p = pSlash+1;

	// look for item unit price
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT unitPrice;
	pSlash = getMoneyAmount( p, unitPrice );
	if( pSlash == NULL )
		return false;

	if( *(p-1) == '-' )
	{
		unitPrice.dollar = -unitPrice.dollar;
		unitPrice.cent = -unitPrice.cent;
	}

	p = pSlash+1;

	MONEY_AMOUNT price;

	// if it is invisible item, return now.
	if( strstr( p, "*.*" ) != NULL )
	{
		memset( &price, 0, sizeof(MONEY_AMOUNT) );

		m_receiver->setItem( m_itemName, unitPrice, 0, price ); 

		return true;
	}

	// look for price
	p = nextDigit( p );
	if( p == NULL )
		return false;

	if( getMoneyAmount( p, price ) == NULL )
		return false;

	if( *(p-1) == '-' )
	{
		price.dollar = -price.dollar;
		price.cent = -price.cent;
	}

	// look for quantity
	p = strchr( pStart, '*' );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if( p == NULL )
		return false;

	int quantity = atoi( p );

	m_receiver->setItem( m_itemName, unitPrice, quantity, price ); 
	
	return true;
}

bool CEpsonIStore0SParser::setItemVoided( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// *REVERSE ROUNDING OFF DISCOUNT    -11.50

	// look for amount
	p = strchr( p, '-' );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if ( p == NULL ) 
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setItemVoided( amount ); 

	return true;
}

bool CEpsonIStore0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL                    152.70

	// EXMAPLE 2: invisible item
	// TOTAL                   *****.**

	MONEY_AMOUNT amount;
	if( strstr( p, "*.*" ) != NULL )
		memset( &amount, 0, sizeof(MONEY_AMOUNT) );
	else
	{
		// look for total
		p = nextDigit( p );
		if( p == NULL )
			return false;
		
		if( getMoneyAmount( p, amount ) == NULL )
			return false;
	}

	m_receiver->setSubtotal( amount ); 

	return true;
}

bool CEpsonIStore0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL AMOUNT                      723.35
           
	// EXMAPLE 2: invisible item
	// TOTAL AMOUNT                    *****.**

	MONEY_AMOUNT amount;
	if( strstr( p, "*.*" ) != NULL )
		memset( &amount, 0, sizeof(MONEY_AMOUNT) );
	else
	{
		// look for total
		p = nextDigit( p );
		if( p == NULL )
			return false;
		
		if( getMoneyAmount( p, amount ) == NULL )
			return false;
	}

	m_receiver->setTotal( amount ); 

	return true;
}

bool CEpsonIStore0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"Cash",			CASH,	// Cash                  160.00
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

bool CEpsonIStore0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE                   76.65

	// EXMAPLE 2:
	// CHANGE                *****.**

	MONEY_AMOUNT amount;
	if( strstr( p, "*.*" ) != NULL )
		memset( &amount, 0, sizeof(MONEY_AMOUNT) );
	else
	{
		// look for change
		p = nextDigit( p );
		if( p == NULL )
			return false;

		if( getMoneyAmount( p, amount ) == NULL )
			return false;
	}

	m_receiver->setChange( amount ); 

	return true;
}

bool CEpsonIStore0SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	// TRANSACTION VOID !!!             -263.89

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}
