// Pace0SParser.h: interface for the CPace0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACE0SPARSER_H__5A27BA52_9AF8_402A_91DE_F12CF4967F33__INCLUDED_)
#define AFX_PACE0SPARSER_H__5A27BA52_9AF8_402A_91DE_F12CF4967F33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

enum PACE0S_PARSE_STATE
{
	PACE0S_STATE_BEGINNING,
	PACE0S_STATE_WELCOME_GOTTEN,
	PACE0S_STATE_HEAD_PASSED,
	PACE0S_STATE_SALE_ITEM_GOTTEN,
	PACE0S_STATE_SALE_ITEM_MORE_GOTTEN,
	PACE0S_STATE_SUBTOTAL_GOTTEN,
	PACE0S_STATE_TAX_GOTTEN,
	PACE0S_STATE_TOTAL_GOTTEN,
	PACE0S_STATE_PAYMENT_TYPE_GOTTEN,
	PACE0S_STATE_TRANSACTION_VOIDED_GOTTEN,
	PACE0S_STATE_DATE_TIME_GOTTEN = PACE0S_STATE_BEGINNING,
	PACE0S_STATE_TRANSACTION_RESETED_GOTTEN = PACE0S_STATE_BEGINNING,
};

class CPace0SParser : public CParser   
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CPace0SParser( CReceiver *receiver );

	virtual ~CPace0SParser();

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
	*				look for date&time information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTime( char *buffer );

	/*
	* Function:		
	*				look for sale item or item voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItem( char *buffer );

	/*
	* Function:		
	*				look for more information of sale item
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemMore( char *buffer );

	/*
	* Function:		
	*				look for subtotal information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setSubtotal( char *buffer );

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
	*				look for transaction voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTransactionVoided( char *buffer );

	PACE0S_PARSE_STATE m_state;

	bool			m_transactionInformed;

	bool			m_preItemGotten;

private:

	char			m_itemName[48];
	MONEY_AMOUNT	m_itemPrice;
};

#endif // !defined(AFX_PACE0SPARSER_H__5A27BA52_9AF8_402A_91DE_F12CF4967F33__INCLUDED_)
