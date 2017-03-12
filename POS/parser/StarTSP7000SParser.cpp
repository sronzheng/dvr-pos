// StarTSP7000SParser.cpp: implementation of the CStarTSP7000SParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StarTSP7000SParser.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStarTSP7000SParser::CStarTSP7000SParser( CReceiver *receiver )
 : CParser( receiver ),
   m_itemQuantity( 0 ),
   m_state( STARTSP7000S_STATE_BEGINNING )
{
	memset( &m_itemUnitPrice, 0, sizeof(MONEY_AMOUNT) );
	m_itemName[0] = 0;
}

CStarTSP7000SParser::~CStarTSP7000SParser()
{

}

void CStarTSP7000SParser::parseLine( char buf[] )
{
	char *p = buf;

	switch( m_state )
	{
	case STARTSP7000S_STATE_BEGINNING:
		if( setTime( p ) )
			m_state = STARTSP7000S_STATE_DATE_TIME_GOTTEN;

		break;

	case STARTSP7000S_STATE_DATE_TIME_GOTTEN:
		if( setItemHead( p ) )
			m_state = STARTSP7000S_STATE_SALE_ITEM_HEAD_GOTTEN;

		break;

	case STARTSP7000S_STATE_SALE_ITEM_HEAD_GOTTEN:
		if( setItemEnd( p ) )
			m_state = STARTSP7000S_STATE_SALE_ITEM_END_GOTTEN;

		break;
	case STARTSP7000S_STATE_SALE_ITEM_END_GOTTEN:
		if( strstr( p, "Skupaj:" ) != NULL ) 
		{
			if( setTotal( p ) )
				m_state = STARTSP7000S_STATE_TOTAL_GOTTEN;
		}
		else
		{
			if( setItemHead( p ) )
				m_state = STARTSP7000S_STATE_SALE_ITEM_HEAD_GOTTEN;
		}

		break;

	default:
		//never go here.
		break;
	}
}

bool CStarTSP7000SParser::setTime( char *buffer )
{
	char *p = buffer;

	// EXAMPLE:
	// Datum: 22.12.2004 Ura:  8:39

	p = nextDigit( p );
	if( p == NULL )
		return false;

	// look for day
	char *pSlash = strchr( p, '.' );
	if( pSlash == NULL )
		return false;
	
	*pSlash = 0;
	int day = atoi( p );
	
	p = pSlash + 1;

	// look for month
	pSlash = strchr( p, '.' );
	if( pSlash == NULL )
		return false;
	
	*pSlash = 0;
	int month = atoi( p );
	
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
	
	// look for minute
	int minute = atoi( p );

	m_receiver->setTransaction();

	m_receiver->setTime( year, month, day, hour, minute ); 

	return true;
}

bool CStarTSP7000SParser::setItemHead( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// PEPSI STEKL.          3.00 X      250.00                 
	//                                   750.00

	char *pStart = p;

	p = p + strlen( p );

	// look for unit price
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	if( getMoneyAmount( p+1, m_itemUnitPrice ) == NULL )
		return false;

	// check whether seperator 'X' exists between quantity and unit price.
	while( *p == ' ' )
		p--;

	if( *p != 'X' || *(p-1) != ' ' )
		return false;

	*(p-1) = 0;

	// look for quantity
	p = strrchr( pStart, '.' );
	if( p == NULL )
		return false;

	*p = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	m_itemQuantity = atoi( p+1 );

	// look for item name;
	while( *(p-1) == ' ' )
		p--;

	*p = 0;

	strcpy( m_itemName, pStart ); 

	return true;
}

bool CStarTSP7000SParser::setItemEnd( char *buffer )
{
	char *p = buffer;
	
	// EXAMPLE 1: 
	// PEPSI STEKL.          3.00 X      250.00                 
	//                                   750.00

	char *pStart = p;

	p = p + strlen( p );
	
	//look for price.
	p = preDigit( p, pStart );
	if( p == NULL )
		return false;

	*(p+1) = 0;

	p = strrchr( pStart, ' ');
	if( p == NULL )
		return false;

	MONEY_AMOUNT amount;

	if( getMoneyAmount( p+1, amount ) == NULL )
		return false;

	m_receiver->setItem( m_itemName, m_itemUnitPrice, m_itemQuantity, amount ); 

	return true;
}

bool CStarTSP7000SParser::setTotal( char *buffer )
{
	char *p = buffer;
	
	// EXMAPLE 1:
	// Skupaj:     \x1B i11      2,390.00 \x1B i00

	char *pStart = p;
           
	// look for total
	p = strrchr( pStart, '\x1B' );
	if( p == NULL )
		return false;

	*p = 0;

	pStart = strrchr( pStart, ' ' );
	if( pStart == NULL )
		return false;

	p = strrchr( pStart, ',' );
	if( p == NULL )
		p = pStart;

	MONEY_AMOUNT amount;
	if( getMoneyAmount( p+1, amount ) == NULL )
		return false;

	int grade = 1000;

	while( p > pStart )
	{
		if( *p >= '0' && *p <= '9' )
		{
			amount.dollar += ( *p - '0' ) * grade;
			grade *= 10;
		}

		p--;
	}

	m_receiver->setTotal( amount ); 

	return true;
}
