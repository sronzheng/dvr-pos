// Parser.h: interface for the CParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__6ADD1065_9D6D_4218_A925_B8CA9094B94A__INCLUDED_)
#define AFX_PARSER_H__6ADD1065_9D6D_4218_A925_B8CA9094B94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "receiver.h"

/*
* interface to specify the parser of POS sale transaction data.		
*/
class CParser  
{
public:

	/*
	* Function:		
	*				Parse data in buffer and inform it to its receiver object
	* Parameters:
	*	buf:		buffer to be parsed.
	*
	* Return Value:	pointer to the end of unhandled data.
	* Example:
	*
	*	char *parser;
	*	< ... get parser ... >
	*
	*   char buf1[] = "     01-01-2005 12:12 \r"
	*				  " 1  ORANGE JUICE 0.99 \r";
	*   char buf2[] = " 1  APPLE JUICE  0.98 \r"
	*				  "TOTAL";
	*	char buf3[]	=      "           $10.00\r";
	*
	*   char buf[256];
	*
	*	strpcy( buf, buf1 ); 
	*	parser->parse( buf );		
	*
	*	strcat( buf, buf2 );
	*	parser->parse( buf );
	*		
	*	strcat( buf, buf2 );
	*	parser->parse( buf );
	*
	*/
	char* parse( char buf[] );

	/*
	* Function:		
	*				parse buffer line.
	* Parameters:
	*	buf:		pointer to buffer to be parsed.
	*/
	virtual void parseLine( char buf[] ) = 0;

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CParser( CReceiver *receiver, char lineSep = '\r' );

	virtual ~CParser();

	// I need access some functions
	// in unit-test project, for test purpose. 
#ifndef _UNIT_TEST_
protected:
#else
public:
#endif

	/*
	* Function:		
	*				look for position of next digit character(0-9).
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				position of next digit character.
	*/
	char *nextDigit( char *buffer );
	
	/*
	* Function:		
	*				look for position of previous digit character(0-9).
	* Parameters:
	*	bufEnd:		pointer to the end position of buffer to be parsed.
	*	bufStart:	pointer to the start position of buffer to be parsed.
	* Return Value:
	*				position of previous digit character.
	*/
	char *preDigit( char *bufEnd, char *bufStart );

	/*
	* Function:		
	*				look for a amount.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	*	aomount:	used to store amount 
	* Return Value:
	*				next position after amount.
	*/
	char *getMoneyAmount( char *buffer, MONEY_AMOUNT &amount );

	/*
	* Function:		
	*				look for month index from its name.
	* Parameters:
	*	name:		month name
	* Return Value:
	*				month index
	*/
	int getMonth( char name[] );

	CReceiver *m_receiver;

private:

	char		m_lineSep;
};

typedef struct _PAYMENT_KEYSTRING_TYPE_PAIR
{
	char *			key;
	PAYMENT_TYPE	type;
} PAYMENT_KEYSTRING_TYPE_PAIR; 

typedef struct _MONTH_NAME_NUMBER_PAIR
{
	char *	name;
	int		number;
} MONTH_NAME_NUMBER_PAIR;

#endif // !defined(AFX_PARSER_H__6ADD1065_9D6D_4218_A925_B8CA9094B94A__INCLUDED_)
