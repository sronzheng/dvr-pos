// TVS0SParser.h: interface for the CTVS0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TVS0SPARSER_H__0E704270_2FAC_4607_BB66_E97BC490885A__INCLUDED_)
#define AFX_TVS0SPARSER_H__0E704270_2FAC_4607_BB66_E97BC490885A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

/*
* enum type for states of the CTVS0SParser.		
*/
enum TVS0S_PARSE_STATE
{
	TVS0S_STATE_BEGINNING,
	TVS0S_STATE_SALE_ITEM_GOTTEN,
	TVS0S_STATE_TAX_GOTTEN,
	TVS0S_STATE_TOTAL_GOTTEN,
	TVS0S_STATE_PAYMENT_TYPE_GOTTEN,
	TVS0S_STATE_NO_SALE_GOTTEN,
	TVS0S_STATE_SAFE_LOAN_GOTTEN,
	TVS0S_STATE_PAY_OUT_GOTTEN,
	TVS0S_STATE_DATE_TIME_GOTTEN = TVS0S_STATE_BEGINNING,
};

class CTVS0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CTVS0SParser( CReceiver *receiver );

	virtual ~CTVS0SParser();

	// I need access the setXXX function and some member variables
	// in unit-test project, for test purpose. 
#ifndef _UNIT_TEST_
private:
#else
public:
#endif

	/*
	* Refer to CParser
	*/
	virtual void parseLine( char buf[] );

	/*
	* Function:		
	*				look for data and time information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	*	inform:		indicate whether inform receiver object, 
	*				when time item found.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTime( char *buffer, bool inform = true );

	/*
	* Function:		
	*				look for sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItem( char *buffer );

	/*
	* Function:		
	*				look for tax information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTax( char *buffer );

	/*
	* Function:		
	*				look for total information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTotal( char *buffer );

	/*
	* Function:		
	*				look for payment type. 
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setPaymentType( char *buffer );

	/*
	* Function:		
	*				look for position of next digit character(0-9).
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				position of next digit character.
	*/

	/*
	* Function:		
	*				look for last digit character(0-9) in a buffer.
	* Parameters:
	*	bufEnd:			pointer to end position of buffer.
	*	bufStart:		pointer to start position of buffer.
						the character in the end position is not a part of buffer.
	* Return Value:
	*				position of last digit character.
	*/

	TVS0S_PARSE_STATE m_state;

	bool m_transactionInformed;

};

#endif // !defined(AFX_TVS0SPARSER_H__0E704270_2FAC_4607_BB66_E97BC490885A__INCLUDED_)
