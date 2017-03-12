// Gilbarco0SParser.cpp: implementation of the CGilbarco0SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gilbarco0SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGilbarco0SParser::CGilbarco0SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_state( GILBARCO0S_STATE_BEGINNING ),
   m_itemHeadGotten( false ),
   m_transactionInformed( false )
{
}

CGilbarco0SParser::~CGilbarco0SParser()
{

}

void CGilbarco0SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN:
		if( strstr( p, "CHANGE" ) != NULL ) 
		{
			if( setChange( p ) )
			{
				m_state = GILBARCO0S_STATE_CHANGE_GOTTEN;
				break;
			}
		}
		else
		{	
			if( setPaymentType( p ) )
			{
				m_state = GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN;
				break;
			}
		}

		// go through.
		// if it is not a change item or a payment item, try to handle it as a time item.

		//break;

	case GILBARCO0S_STATE_BEGINNING:
		if( strstr( p, "PAYIN" ) != NULL ) 
			m_state = GILBARCO0S_STATE_PAY_IN_GOTTEN;
		else
		{
			GILBARCO0S_ITEM_TYPE type = setItem( p );

			if( type == GILBARCO0S_HEAD_ITEM )
				m_state = GILBARCO0S_STATE_SALE_ITEM_HEAD_GOTTEN;
			else if( type == GILBARCO0S_MAIN_ITEM )
				m_state = GILBARCO0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case GILBARCO0S_STATE_SALE_ITEM_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
		}
		else if( strstr( p, "TL/NOTAX" ) != NULL ) 
		{
			if( setSubtotal( p ) )
				m_state = GILBARCO0S_STATE_SUBTOTAL_GOTTEN;
		}
		else
		{
			GILBARCO0S_ITEM_TYPE type = setItem( p );

			if( type == GILBARCO0S_HEAD_ITEM )
				m_state = GILBARCO0S_STATE_SALE_ITEM_HEAD_GOTTEN;
			else if( type == GILBARCO0S_MAIN_ITEM )
				m_state = GILBARCO0S_STATE_SALE_ITEM_GOTTEN;
		}

		break;

	case GILBARCO0S_STATE_SALE_ITEM_HEAD_GOTTEN:
		if( setItem( p ) == GILBARCO0S_MAIN_ITEM )
			m_state = GILBARCO0S_STATE_SALE_ITEM_GOTTEN;
		
		break;


	case GILBARCO0S_STATE_SUBTOTAL_GOTTEN:
		if( strstr( p, "TAX PD" ) != NULL ) 
		{
			if( setTax( p ) )
				m_state = GILBARCO0S_STATE_TAX_GOTTEN;
		}

		break;

	case GILBARCO0S_STATE_TAX_GOTTEN:
		if( strstr( p, "TOTAL" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = GILBARCO0S_STATE_TOTAL_GOTTEN;
		}

		break;

	case GILBARCO0S_STATE_TOTAL_GOTTEN:
		if( setPaymentType( p ) )
			m_state = GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN;

		break;

	case GILBARCO0S_STATE_PAY_IN_GOTTEN:
		if( strstr( p, "CASH" ) != NULL ) 
			m_state = GILBARCO0S_STATE_CASH_GOTTEN;

		break;

	default:
		//never go here.
		break;
	}
}

GILBARCO0S_ITEM_TYPE CGilbarco0SParser::setItem( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: sale item
	// SANTE FE R  $0.99 

	// EXAMPLE 1: sale item head: quantity and unit price.
	// 2 X       $3.550 

	char *pStart = buffer;

	// look for price or unit price.
	p = strchr( pStart, '$');
	if( p == NULL )
		return GILBARCO0S_NULL_ITEM;

	MONEY_AMOUNT price;
	if( getMoneyAmount( p+1, price ) == NULL )
		return GILBARCO0S_NULL_ITEM;

	// discard space characters at tail, preparing to look for item name
	while( *(p-1) == ' ' )
		p--;

	// if we encounter "X ", it may be a quantity line. try to find it.
	// But don't modify it, before we can be sure that.
	if( *(p-1) == 'X' && *(p-2) == ' ' )
	{
		char *pHead = p-2;

		// look for quantity
		while( *pHead == ' ' )
			pHead--;

		if( *pHead < '0' || *pHead > '9' )
			goto main_item;

		while( *pHead >= '0' && *pHead <= '9' )
			pHead--;

		if( *pHead != ' ' )
			goto main_item;

		m_itemQuantity = atoi( pHead+1 );
		memcpy( &m_itemUnitPrice, &price, sizeof(MONEY_AMOUNT));

		m_itemHeadGotten = true;

		return GILBARCO0S_HEAD_ITEM;
	}

main_item:

	*p = 0;

	if( ! m_transactionInformed )
	{
		m_receiver->setTransaction(); 

		m_transactionInformed = true;
	}

	if( m_itemHeadGotten )
	{
		m_receiver->setItem( pStart, m_itemUnitPrice, m_itemQuantity, price );   
	}
	else
	{
		MONEY_AMOUNT unitPrice;
		unitPrice.dollar = 0;
		unitPrice.cent = 0;

		m_receiver->setItem( pStart, unitPrice, 1, price );   
	}

	m_itemHeadGotten = false;

	return GILBARCO0S_MAIN_ITEM;
}

bool CGilbarco0SParser::setSubtotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TL/NOTAX     $1.98

	// look for total
	p = nextDigit( p );
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p, amount ) == NULL )
		return false;

	m_receiver->setSubtotal( amount ); 

	m_transactionInformed = false;

	return true;
}

bool CGilbarco0SParser::setTax( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TAX PD       $0.05

	// EXMAPLE 2:
	// TAX01                    .27

	// EXMAPLE 2:
	// TAX01                  -2.58

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

bool CGilbarco0SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// TOTAL        $2.03
           
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

bool CGilbarco0SParser::setPaymentType( char *buffer )
{
	char *p = buffer;

	static PAYMENT_KEYSTRING_TYPE_PAIR paymentType[] = {
		"CASH",			CASH,				// CASH        $10.10
		"DEBIT",		ATM,				// DEBIT        $6.70
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

bool CGilbarco0SParser::setChange( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// CHANGE       $8.07

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
