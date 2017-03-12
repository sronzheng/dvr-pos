// Parser.cpp: implementation of the CParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parser.h"

#include <stdlib.h>

static MONTH_NAME_NUMBER_PAIR month_pairs[] = 
{
	"JAN",	1,
	"FEB",	2,
	"MAR",	3,
	"APR",	4,
	"MAY",	5,
	"JUN",	6,
	"JUL",	7,
	"AUG",	8,
	"SEP",	9,
	"OCT",	10,
	"NOV",	11,
	"DEC",	12,
}; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParser::CParser( CReceiver *receiver, char lineSep )
 : m_receiver( receiver ),
   m_lineSep( lineSep )
{

}

CParser::~CParser()
{

}

char* CParser::parse( char buf[] )
{
	static const int LINE_MAXLEN = 128;

	// divide buffer into lines, and feed parser line by line.
	char *pStart = buf;
	char *pEnd = strchr( pStart, m_lineSep );
	
	while( pEnd != NULL )
	{
		*pEnd = 0;
		
		parseLine( pStart );
		
		pStart = pEnd + 1;
		pEnd = strchr( pStart, m_lineSep );
	}

	strcpy( buf, pStart );

	int len = strlen( buf );
	if( len > LINE_MAXLEN )
		buf[0] = 0;

	return buf + len ;
}

char *CParser::nextDigit( char *buffer )
{
	char *p = buffer;

	while( *p )
	{
		//if ( isdigit( *p ) )	
		if ( *p >= '0' && *p <= '9' )
			return p;
		++p;
	}

	return NULL;
}

char *CParser::preDigit( char *bufEnd, char *bufStart )
{
	if ( bufEnd <= bufStart )
		return NULL;

	char *p = bufEnd - 1;

	while( p >= bufStart )
	{
		//if ( isdigit( *p ) )	
		if ( *p >= '0' && *p <= '9' )
			return p;
		--p;
	}

	return NULL;
}

char *CParser::getMoneyAmount( char *buffer, MONEY_AMOUNT &amount  )
{
	char *p = buffer;

	char *pSlash = strchr( p, '.' );
	if ( pSlash == NULL )
		return NULL;

	char *pDollar = nextDigit( p );
	if ( pDollar == NULL ) 
		return NULL;

	if ( pDollar < pSlash )		// amount like "2.00"
	{
		*pSlash = 0;
		amount.dollar = atoi( pDollar );
	}
	else						// amount like ".78"
		amount.dollar = 0;

	char *pCent = pSlash + 1;

	// Only two digits needed for cent part.
	char oldChar;
	if( *pCent != 0 && *(pCent+1) != 0 )
	{
		oldChar = *(pCent+2);
		*(pCent+2) = 0;
	}

	amount.cent = atoi( pCent );

	if( *pCent != 0 && *(pCent+1) != 0 )
		*(pCent+2) = oldChar;

	return pCent + 2;
}

int CParser::getMonth( char name[] )
{
	for( int i = 0; 
	     i < sizeof(month_pairs) / sizeof(month_pairs[0]);
		 i++ )
	{
		if( strcmp( month_pairs[i].name, name ) == 0  )
			return month_pairs[i].number;
	}

	return -1;
}
