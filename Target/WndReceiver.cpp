// WndReceiver.cpp: implementation of the CWndReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WndReceiver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWndReceiver::CWndReceiver( CListBox *view )
: m_view(view)
{

}

CWndReceiver::~CWndReceiver()
{

}

void CWndReceiver::setTransaction()
{
	m_view->AddString(" "); 
	m_view->AddString(" "); 
	m_view->AddString("REG"); 
	m_view->AddString("------------------------"); 
}

void CWndReceiver::setTime( int year, 
						    int month, 
							int day, 
							int hour, 
							int minute )
{
	static char format[] = "REG  %02d-%02d-%4d  %02d:%02d";

	char buf[128];
	sprintf( buf, format, month, day, year, hour, minute );

	m_view->AddString( buf ); 
}

void CWndReceiver::setCardType( char typeName[] )
{
}

void CWndReceiver::setItem( char name[],
						    const MONEY_AMOUNT &unitPrice,
							int quantity,					  
							const MONEY_AMOUNT &price )
{
	// only show the front 16 characters.
	if( strlen( name ) > 16 )
		name[16] = '\0';

	bool refund = FALSE;
	if( price.dollar < 0 || price.cent < 0)
		refund = TRUE;

	char buf[64];

	if( refund )
	{
		static char format[] = "%2d  %16s    -%d.%02d";
		sprintf( buf, format, quantity, name,  abs( price.dollar ), abs( price.cent ) );
	}
	else
	{
		static char format[] = "%2d  %16s    %d.%02d";
		sprintf( buf, format, quantity, name,  price.dollar, price.cent );
	}

	m_view->AddString( buf ); 
}

void CWndReceiver::setItemVoided( const MONEY_AMOUNT &amount )
{
	char buf[] = "   *ITEM VOIDED*   ";

	m_view->AddString( buf ); 
}

void CWndReceiver::setSubtotal( const MONEY_AMOUNT &amount )
{
	bool refund = FALSE;
	if( amount.dollar < 0 || amount.cent < 0)
		refund = TRUE;

	char buf[64];

	if( refund )
	{
		static char format[] = "        SUB-TOTAL: -%d.%02d";
		sprintf( buf, format, abs( amount.dollar ), abs( amount.cent ) );
	}
	else
	{
		static char format[] = "        SUB-TOTAL: %d.%02d";
		sprintf( buf, format, amount.dollar, amount.cent );
	}

	m_view->AddString( buf ); 
}

void CWndReceiver::setTax( const MONEY_AMOUNT &amount )
{
	bool refund = FALSE;
	if( amount.dollar < 0 || amount.cent < 0)
		refund = TRUE;

	char buf[64];

	if( refund )
	{
		static char format[] = "              TAX: -%d.%02d";
		sprintf( buf, format, abs( amount.dollar ), abs( amount.cent ) );
	}
	else
	{
		static char format[] = "              TAX: %d.%02d";
		sprintf( buf, format, amount.dollar, amount.cent );
	}

	m_view->AddString( buf ); 
}

void CWndReceiver::setTotal( const MONEY_AMOUNT &amount )
{
	bool refund = FALSE;
	if( amount.dollar < 0 || amount.cent < 0)
		refund = TRUE;

	char buf[64];

	if( refund )
	{
		static char format[] = "            TOTAL: -%d.%02d";
		sprintf( buf, format, abs( amount.dollar ), abs( amount.cent ) );
	}
	else
	{
		static char format[] = "            TOTAL: %d.%02d";
		sprintf( buf, format, amount.dollar, amount.cent );
	}

	m_view->AddString( buf ); 
}

void CWndReceiver::setPaymentType( PAYMENT_TYPE type, 
								   const MONEY_AMOUNT &amount )
{
	char typeName[32];

	switch( type )
	{
	case CASH:
		strcpy(typeName, "CASH");
		break;
	case CHECK:
		strcpy(typeName, "CHECK");
		break;
	case CREDIT:
		strcpy(typeName, "CREDIT");
		break;
	case ATM:
		strcpy(typeName, "ATM");
		break;
	case GIFT_CERTIFICATE:
		strcpy(typeName, "GIFT CERTIFICATE");
		break;
	default:
		break;
	}

	char buf[64];

	if( amount.dollar < 0 || amount.cent < 0)
	{
		static char format[] = "%17s: -%d.%02d";
		sprintf( buf, format, typeName, abs(amount.dollar), abs(amount.cent) );
	}
	else
	{
		static char format[] = "%17s: %d.%02d";
		sprintf( buf, format, typeName, amount.dollar, amount.cent );
	}

	m_view->AddString( buf ); 
}

void CWndReceiver::setChange( const MONEY_AMOUNT &amount )
{
	static char format[] = "           CHANGE: %d.%02d";

	char buf[64];
	sprintf( buf, format, amount.dollar, amount.cent );

	m_view->AddString( buf ); 
}

void CWndReceiver::setRefund( const MONEY_AMOUNT &amount )
{
	static char format[] = "           REFUND: %d.%02d";

	char buf[64];
	sprintf( buf, format, amount.dollar, amount.cent );

	m_view->AddString( buf ); 
}

void CWndReceiver::setTransactionVoided()
{
	static char buf[] = " **TRANSACTION VOIDED** ";

	m_view->AddString( buf ); 
}
