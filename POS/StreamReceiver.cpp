// StreamReceiver.cpp: implementation of the CStreamReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StreamReceiver.h"

#include "posstruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef struct _PAYMENT_TYPE_KEY_PAIR
{
	PAYMENT_TYPE	type;
	char *			key;
} PAYMENT_TYPE_KEY_PAIR;

//===========================================================================
//  SysTime2UINT64 
//===========================================================================
static void sysTime2UINT64( const SYSTEMTIME *sysTime, UINT64 *time64 )
{
    FILETIME fileTime;
    SystemTimeToFileTime( sysTime, &fileTime );

	ULONGLONG temp;

    // Copy the time into a quadword.
	temp = (((ULONGLONG) fileTime.dwHighDateTime) << 32) + fileTime.dwLowDateTime;
    *time64 = temp / 10000;
}

CStreamReceiver::CStreamReceiver( CBinaryStream &stream )
 : m_stream( stream )
{

}

CStreamReceiver::~CStreamReceiver()
{
}

void CStreamReceiver::setTransaction()
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Transaction" );  

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setTime( int year, 
							   int month, 
							   int day, 
							   int hour, 
							   int minute )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "DateTime" );  

	// set time
	SYSTEMTIME sysTime;
	memset( &sysTime, 0, sizeof(sysTime) );
	sysTime.wYear = year;
	sysTime.wMonth = month;
	sysTime.wDay = day;
	sysTime.wHour = hour;
	sysTime.wMinute = minute;

	sysTime2UINT64( &sysTime, &data.dateTime );  

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setCardType( char typeName[] )
{
}

void CStreamReceiver::setItem( char name[],
							   const MONEY_AMOUNT &unitPrice,
							   int quantity,					  
							   const MONEY_AMOUNT &price )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Item" );  

	// set item name
	strcpy( data.itemData.name, name );

	// set unit price
	data.itemData.unitPrice = unitPrice.dollar * 100 + unitPrice.cent;  

	// set quantity
	data.itemData.quantity = quantity;  

	// set price
	data.itemData.price = price.dollar * 100 + price.cent;  

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setItemVoided( const MONEY_AMOUNT &amount )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Item Voided" );  

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setSubtotal( const MONEY_AMOUNT &amount )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Subtotal" );  

	// set amount
	data.amount = amount.dollar * 100 + amount.cent;  

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setTax( const MONEY_AMOUNT &amount )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Tax" );  

	// set amount
	data.amount = amount.dollar * 100 + amount.cent; 

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setTotal( const MONEY_AMOUNT &amount )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Total" );  

	// set amount
	data.amount = amount.dollar * 100 + amount.cent; 

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setPaymentType( PAYMENT_TYPE type, 
									  const MONEY_AMOUNT &amount )
{
	static PAYMENT_TYPE_KEY_PAIR paymentType[] = 
	{
		CASH,				"Cash",
		CHECK,				"Check",
		CREDIT,				"Credit",
		ATM,				"ATM",
		GIFT_CERTIFICATE,	"Gift certificate"
	};

	char *keyword = NULL;

	for( int i = 0; 
	     i < sizeof( paymentType ) / sizeof( paymentType[0] ); 
		 i++ )
	{
			 if( type == paymentType[i].type ) 
			 {
				 keyword = paymentType[i].key; 
				 break;
			 }
	}

	if( keyword == NULL )
		return ;

	POS_DATA data;

	// set keyword
	strcpy( data.keyword, keyword );  

	// set amount
	data.amount = amount.dollar * 100 + amount.cent; 

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setChange( const MONEY_AMOUNT &amount )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Change" );  

	// set amount
	data.amount = amount.dollar * 100 + amount.cent; 

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setRefund( const MONEY_AMOUNT &amount )
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Refund" );  

	// set amount
	data.amount = amount.dollar * 100 + amount.cent; 

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}

void CStreamReceiver::setTransactionVoided()
{
	POS_DATA data;

	// set keyword
	strcpy( data.keyword, "Transaction Voided" );  

	m_stream.write( &data, sizeof( POS_DATA ) ); 
}
