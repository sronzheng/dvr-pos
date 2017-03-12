// XmlFileReceiver.cpp: implementation of the CXmlFileReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlFileReceiver.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CXmlFileReceiver::PopLowerLevelBack( ITEM_LEVEL curLevel)
{
	while( ! m_backs.empty() )
	{
		BACK_LEVEL *itemLevel = m_backs.top();

		if( itemLevel->level >= curLevel )
		{
			m_of << itemLevel->back; 

			delete itemLevel;
			m_backs.pop();
		}
		else 
			break;
	}
}

CXmlFileReceiver::CXmlFileReceiver( std::ofstream &of )
 : m_of( of )
{
	ITEM_LEVEL curLevel = TOP;

	m_of << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";

	m_of << "<Transactions>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Transactions>");

	m_backs.push( itemLevel );
}

CXmlFileReceiver::~CXmlFileReceiver()
{
	ITEM_LEVEL curLevel = TOP;

	PopLowerLevelBack( curLevel );
}

void CXmlFileReceiver::setTransaction()
{
	ITEM_LEVEL curLevel = TRANSACTION;

	PopLowerLevelBack( curLevel );

	//write transaction
	m_of << "<Transaction>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Transaction>");

	m_backs.push( itemLevel );
}

void CXmlFileReceiver::setTime( int year, 
								int month, 
								int day, 
								int hour, 
								int minute )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	char tm[60];
	sprintf( tm, "%02d-%02d-%d %02d:%02d", month, day, year, hour, minute);

	// write Date&Time item
	m_of << "<DateTime>";
	m_of << tm;
	m_of << "</DateTime>";
}

void CXmlFileReceiver::setCardType( char typeName[] )
{
}

void CXmlFileReceiver::setItem( char name[],
							    const MONEY_AMOUNT &unitPrice,
								int quantity,					  
								const MONEY_AMOUNT &price )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	// write "sale item" item
	m_of << "<Item>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Item>");

	m_backs.push( itemLevel );

	// write quantity child item
	m_of << "<Quantity>";
	m_of << quantity;
	m_of << "</Quantity>";

	// write unit price child item
	m_of << "<UnitPrice>";
	m_of << unitPrice.dollar * 100 + unitPrice.cent ;
	m_of << "</UnitPrice>";

	// write "item name" child item
	m_of << "<ItemName>";
	m_of << name;
	m_of << "</ItemName>";

	// write price child item
	m_of << "<Price>";
	m_of << price.dollar * 100 + price.cent;
	m_of << "</Price>";
}

void CXmlFileReceiver::setItemVoided( const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<ItemVoided>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</ItemVoided>");

	m_backs.push( itemLevel );
}

void CXmlFileReceiver::setSubtotal( const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<Subtotal>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Subtotal>");

	m_backs.push( itemLevel );

	// write amount item
	m_of << "<Amount>";
	m_of << amount.dollar * 100 + amount.cent;
	m_of << "</Amount>";
}

void CXmlFileReceiver::setTax( const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<Tax>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Tax>");

	m_backs.push( itemLevel );

	// write amount item
	m_of << "<Amount>";
	m_of << amount.dollar * 100 + amount.cent;
	m_of << "</Amount>";
}

void CXmlFileReceiver::setTotal( const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<Total>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Total>");

	m_backs.push( itemLevel );

	// write amount item
	m_of << "<Amount>";
	m_of << amount.dollar * 100 + amount.cent;
	m_of << "</Amount>";
}

void CXmlFileReceiver::setPaymentType( PAYMENT_TYPE type, 
									   const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	char typeName[32];

	switch( type )
	{
	case CASH:
		strcpy(typeName, "Cash");
		break;
	case CHECK:
		strcpy(typeName, "Check");
		break;
	case CREDIT:
		strcpy(typeName, "Credit");
		break;
	case ATM:
		strcpy(typeName, "ATM");
		break;
	case GIFT_CERTIFICATE:
		strcpy(typeName, "Giftcertificate");
		break;
	default:
		break;
	}

	m_of << "<" << typeName << ">";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	sprintf( itemLevel->back, "</%s>", typeName );

	m_backs.push( itemLevel );

	// write amount item
	m_of << "<Amount>";
	m_of << amount.dollar * 100 + amount.cent;
	m_of << "</Amount>";
}

void CXmlFileReceiver::setChange( const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<Change>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Change>");

	m_backs.push( itemLevel );

	// write amount item
	m_of << "<Amount>";
	m_of << amount.dollar * 100 + amount.cent;
	m_of << "</Amount>";
}

void CXmlFileReceiver::setRefund( const MONEY_AMOUNT &amount )
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<Refund>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</Refund>");

	m_backs.push( itemLevel );

	// write amount item
	m_of << "<Amount>";
	m_of << amount.dollar * 100 + amount.cent;
	m_of << "</Amount>";
}

void CXmlFileReceiver::setTransactionVoided()
{
	ITEM_LEVEL curLevel = ITEM;

	PopLowerLevelBack( curLevel );

	m_of << "<TransactionVoided>";

	BACK_LEVEL *itemLevel = new BACK_LEVEL();

	itemLevel->level = curLevel;
	strcpy( itemLevel->back, "</TransactionVoided>");

	m_backs.push( itemLevel );
}
