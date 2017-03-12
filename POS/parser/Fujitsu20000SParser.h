// Fujitsu20000SParser.h: interface for the CFujitsu20000SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUJITSU20000SPARSER_H__56B6137E_CB7A_4B41_8E61_44520E4EF277__INCLUDED_)
#define AFX_FUJITSU20000SPARSER_H__56B6137E_CB7A_4B41_8E61_44520E4EF277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

/*
* enum type for states of the CFujitsu20000SParser.		
*/
enum FUJITSU20000S_PARSE_STATE
{
	FUJITSU20000S_STATE_BEGINNING,
	FUJITSU20000S_STATE_SALE_ITEM_GOTTEN,
	FUJITSU20000S_STATE_BALANCE_DUE_GOTTEN,
	FUJITSU20000S_STATE_PAYMENT_TYPE_GOTTEN,
	FUJITSU20000S_STATE_CHANGE_GOTTEN,
	FUJITSU20000S_STATE_SUBTOTAL_GOTTEN,
	FUJITSU20000S_STATE_TAX_GOTTEN,
	FUJITSU20000S_STATE_TOTAL_GOTTEN,
	FUJITSU20000S_STATE_TRANSACTION_VOIDED_GOTTEN,
	FUJITSU20000S_STATE_DATE_TIME_GOTTEN = FUJITSU20000S_STATE_BEGINNING,
};

/*
* the parser class for Fujitsu20000S POS sale transaction data.		
*/
class CFujitsu20000SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CFujitsu20000SParser( CReceiver *receiver );

	virtual ~CFujitsu20000SParser();

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
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTime( char *buffer );

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
	*				look for change. 
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setChange( char *buffer );

	/*
	* Function:		
	*				look for transaction voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTransactionVoided( char *buffer );

	FUJITSU20000S_PARSE_STATE m_state;

	bool m_transactionInformed;
};

#endif // !defined(AFX_FUJITSU20000SPARSER_H__56B6137E_CB7A_4B41_8E61_44520E4EF277__INCLUDED_)
