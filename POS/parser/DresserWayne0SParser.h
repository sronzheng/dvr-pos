// DresserWayne0SParser.h: interface for the CDresserWayne0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRESSERWAYNE0SPARSER_H__A516675A_84B1_41C7_9491_F76BF2475441__INCLUDED_)
#define AFX_DRESSERWAYNE0SPARSER_H__A516675A_84B1_41C7_9491_F76BF2475441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CDresserWayne0SParser.		
*/
enum DRESSERWAYNE0S_PARSE_STATE
{
	DRESSERWAYNE0S_STATE_BEGINNING,
	DRESSERWAYNE0S_STATE_SALE_ITEM_HEAD_GOTTEN,
	DRESSERWAYNE0S_STATE_SALE_ITEM_GOTTEN,
	DRESSERWAYNE0S_STATE_SUBTOTAL_GOTTEN,
	DRESSERWAYNE0S_STATE_TAX_GOTTEN,
	DRESSERWAYNE0S_STATE_TOTAL_GOTTEN,
	DRESSERWAYNE0S_STATE_PAYMENT_TYPE_GOTTEN,
	DRESSERWAYNE0S_STATE_CHANGE_GOTTEN,
	DRESSERWAYNE0S_STATE_TRANSACTION_VOIDED_GOTTEN,
	DRESSERWAYNE0S_STATE_PAID_IN_GOTTEN,
	DRESSERWAYNE0S_STATE_SAFE_DROP_GOTTEN,
	DRESSERWAYNE0S_STATE_DATE_TIME_GOTTEN = DRESSERWAYNE0S_STATE_BEGINNING,
};

/*
* the parser class for DresserWayne0S POS sale transaction data.		
*/
class CDresserWayne0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CDresserWayne0SParser( CReceiver *receiver );

	virtual ~CDresserWayne0SParser();

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
	bool setTime( char *buffer, bool inform = true );

	/*
	* Function:		
	*				look for the head item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemHead( char *buffer );

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
	*				look for item voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemVoided( char *buffer );

	/*
	* Function:		
	*				look for transaction voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTransactionVoided( char *buffer );

	DRESSERWAYNE0S_PARSE_STATE m_state;

	bool m_transactionInformed;

private:

	bool			m_itemHeadGotten;
	char			m_itemQuantity;
	MONEY_AMOUNT	m_itemUnitPrice;
};

#endif // !defined(AFX_DRESSERWAYNE0SPARSER_H__A516675A_84B1_41C7_9491_F76BF2475441__INCLUDED_)
