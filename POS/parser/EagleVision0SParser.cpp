// CEagleVision0SParser.cpp: implementation of the CCasioParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EagleVision0SParser.h"

#include <string.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEagleVision0SParser::CEagleVision0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( EAGLEVISION0S_STATE_BEGINNING )
{
}

CEagleVision0SParser::~CEagleVision0SParser()
{

}

void CEagleVision0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case EAGLEVISION0S_STATE_BEGINNING:
		if( setTime( p ) )
			m_state = EAGLEVISION0S_STATE_DATE_TIME_GOTTEN;

		break;

	case EAGLEVISION0S_STATE_DATE_TIME_GOTTEN:
		if( strstr( p, "CARD TYPE:" ) != NULL )
				m_state = EAGLEVISION0S_STATE_CARD_TYPE_GOTTEN;
		else
		{
			if( setItemHead( p ) )
				m_state = EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}

		break;

	case EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN:
		if( setItemEnd( p ) )
			m_state = EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN;

		break;

	case EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "SUB-TOTAL:" ) != NULL )
		{
			if( setSubtotal( p ) )
				m_state = EAGLEVISION0S_STATE_SUBTOTAL_GOTTEN;
		}
		else if( strstr( p, "*ITEM VOIDED*" ) != NULL )
		{
			setItemVoided( p );

			// keep state
		}
		else if( strstr( p, "*TRANSACTION VOIDED*" ) != NULL )
		{
			if( setTransactionVoided( p ) )
				m_state = EAGLEVISION0S_STATE_TRANSACTION_VOIDED_GOTTEN;
		}else
		{
			if ( setItemHead( p ) )
				m_state = EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}

		break;

	case EAGLEVISION0S_STATE_SUBTOTAL_GOTTEN:
		if( setTotal( p ) )
			m_state = EAGLEVISION0S_STATE_TOTAL_GOTTEN;

		break;

	case EAGLEVISION0S_STATE_TOTAL_GOTTEN:
		if( strstr( p, "REFUND:" ) != NULL )
		{
			if( setRefund( p ) )
				m_state = EAGLEVISION0S_STATE_REFUND_GOTTEN;
		}
		else
		{
			if( setPaymentType( p ) )
				m_state = EAGLEVISION0S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case EAGLEVISION0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "====>>" ) != NULL )
			m_state = EAGLEVISION0S_STATE_END_FLAG_GOTTEN;
		else
		{
			if( setPaymentType( p ) )
				m_state = EAGLEVISION0S_STATE_PAYMENT_TYPE_GOTTEN;
		}

		break;

	case EAGLEVISION0S_STATE_REFUND_GOTTEN:
	case EAGLEVISION0S_STATE_TRANSACTION_VOIDED_GOTTEN:
	case EAGLEVISION0S_STATE_CARD_TYPE_GOTTEN:
		if( strstr( p, "====>>" ) != NULL )
			m_state = EAGLEVISION0S_STATE_END_FLAG_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}

}

bool CEagleVision0SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// 12/13/05  14:15  SYSTEM     551 SALE

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

bool CEagleVision0SParser::setItemHead( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: ( Sale Transaction)
	// 1                 1   EA     2.00 EA					// lst line
	// TEST SKU                        2.00					// 2nd line
	
	// EXAMPLE 2: ( Refund Transaction )
	// 1                 1   EA     2.00 EA					// lst line
	// TEST SKU                     (N 02      -4.02(N 01	// 2nd line
	
	p = strchr( p, ' ' );
	if( p == NULL )
		return false;
	
	// look for quantity
	p = nextDigit( p );
	if ( p == NULL ) 
		return false;
	
	char *pSlash = strchr( p, ' ' );
	if( pSlash == NULL )
		return false;
	
	*pSlash = 0;
	m_itemQuantity = atoi( p );
	
	p = pSlash + 1;
	
	// look for unit price
	p = nextDigit( p );
	if( p == NULL )
		return false;

	if( *(p-1) == '.' )
		p--;
	 
	if( getMoneyAmount( p, m_itemUnitPrice ) == NULL )
		return false;
	
	return true;
}

bool CEagleVision0SParser::setItemEnd( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: ( Sale Transaction)
	// 1                 1   EA     2.00 EA
	// TEST SKU                        2.00
	
	// EXAMPLE 2: ( Refund Transaction )
	// 1                 1   EA     2.00 EA
	// TEST SKU                     (N 02      -4.02(N 01

	char *pStart = buffer;

	// look for price
	char *pSlash = strchr( p, '.' );
	if( pSlash == NULL )
		return false;

	p = pSlash;

	while( *(p-1) >= '0' && *(p-1) <= '9' )
		p--;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	// look for sign of price.
	if( *(p-1) == '-' )
	{
		amount.dollar = -amount.dollar; 
		amount.cent = -amount.cent; 
	}

	// look for item name
	*p = 0;
	pSlash = strrchr( pStart, '(' );
	if( pSlash != NULL )
		p = pSlash - 1;

	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	m_receiver->setItem( pStart, m_itemUnitPrice, m_itemQuantity, amount ); 
	
	return true;
}

bool CEagleVision0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:	Sale
	// SUB-TOTAL:       8.00    TAX:        .70  

	// EXMAPLE 2:	Refund
	// SUB-TOTAL:      -8.00    TAX:       -.70  

	// look for subtotal
	p = nextDigit( p );
	if( p == NULL )
		return false;
	
	if ( *(p-1) == '.' )
		--p;

	MONEY_AMOUNT subtotal;
	char *pNext = getMoneyAmount( p, subtotal );
	if( pNext == NULL )
		return false;

	if( *(p-1) == '-' )
	{
		subtotal.dollar = -subtotal.dollar; 
		subtotal.cent = -subtotal.cent; 
	}

	p = pNext;

	// look for tax
	p = nextDigit( p );
	if( p == NULL )
		return false;
	
	if ( *(p-1) == '.' )
		--p;

	MONEY_AMOUNT tax;
	if( getMoneyAmount( p, tax ) == NULL )
		return false;

	if( *(p-1) == '-' )
	{
		tax.dollar = -tax.dollar; 
		tax.cent = -tax.cent; 
	}

	m_receiver->setSubtotal( subtotal ); 
	m_receiver->setTax( tax ); 

	return true;
}

bool CEagleVision0SParser::setTotal( char *buffer )
{
	char *p = buffer;

	// EXAMPLE 1:  ( Sale Transaction )
	//                     TOTAL:       8.70

	// EXAMPLE 2:  ( Refund Transaction )
	//                     TOTAL: (N 02     -2.19(N 01

	// look for the start position of total
	char *pSign = strchr( p, '-');
	if( pSign != NULL )
		p = pSign + 1;

	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;
	
	if ( *(p-1) == '.' )
		--p;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	if( *(p-1) == '-' )
	{
		amount.dollar = -amount.dollar; 
		amount.cent = -amount.cent; 
	}

	m_receiver->setTotal( amount ); 

	return true;
}

bool CEagleVision0SParser::setCashOfPayment( char *buffer )
{
	char *p = buffer;

	// EXAMPLE 1:
	// CASH TEND:       9.00 CHANGE:        .30

	// look for cash payment
	p = nextDigit( p );
	if ( p == NULL ) 
		return false;

	if ( *(p-1) == '.' )
		--p;
	
	MONEY_AMOUNT cash;
	p = getMoneyAmount( p, cash );
	if( p == NULL )
		return false;

	// look for change
	p = nextDigit( p );
	if ( p == NULL )
		return true;
	
	if ( *(p-1) == '.' )
		--p;
	
	MONEY_AMOUNT change;
	if( getMoneyAmount( p, change ) == NULL )
		return false;
	
	m_receiver->setPaymentType( CASH, cash); 
	m_receiver->setChange( change ); 
	
	return true;
}

bool CEagleVision0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	// if payment by cash, handle it, and stop search routine. 
	if( strstr( p, "CASH TEND:") != NULL )
		return setCashOfPayment( p );

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"   CK AMT: ",	CHECK,				// CK AMT:          8.70
		"   BC AMT: ",	CREDIT,				// BC AMT:          8.70				
		"DEBIT/ATM: ",	ATM,				// DEBIT/ATM:        .97
		"GIFT CERTIF",	GIFT_CERTIFICATE,	// GIFT CERTIFICATE 5.00	
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

			if ( *(p-1) == '.' )
				--p;

			MONEY_AMOUNT amount;
			if( getMoneyAmount( p, amount ) == NULL )
				return false;

			m_receiver->setPaymentType( paymentType[i].type, amount);  
	
			return true;
		}
	}

	return false;
}

bool CEagleVision0SParser::setItemVoided( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// 1              *ITEM VOIDED* [     2.01]

	// look for amount
	p = strchr( p, '[' );
	if( p == NULL )
		return false;

	p = nextDigit( p );
	if ( p == NULL ) 
		return false;

	if ( *(p-1) == '.' )
		--p;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setItemVoided( amount ); 

	return true;
}

bool CEagleVision0SParser::setRefund( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// REFUND:       2.19
	
	// look for amount
	p = nextDigit( p );
	if ( !p ) 
		return false;
	
	if ( *(p-1) == '.' )
		--p;
	
	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;
	
	m_receiver->setRefund( amount );  
	
	return true;
}

bool CEagleVision0SParser::setTransactionVoided( char *buffer )
{
	// EXAMPLE:
	// **TRANSACTION VOIDED**

	// do nothing but just inform receiver.
	m_receiver->setTransactionVoided();
	
	return true;
}

