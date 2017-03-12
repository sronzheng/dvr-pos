// EpsonIStore0S.h: interface for the CEpsonIStore0S class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPSONISTORE0S_H__ED942885_4F7F_45FC_9EEB_7F4B995F77BD__INCLUDED_)
#define AFX_EPSONISTORE0S_H__ED942885_4F7F_45FC_9EEB_7F4B995F77BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

/*
* enum type for states of the CEpsonIStore0SParser.		
*/
enum EPSONISTORE0S_PARSE_STATE
{
	EPSONISTORE0S_STATE_BEGINNING,
	EPSONISTORE0S_STATE_DATE_TIME_GOTTEN,
	EPSONISTORE0S_STATE_SALE_ITEM_GOTTEN,
	EPSONISTORE0S_STATE_SUBTOTAL_GOTTEN,
	EPSONISTORE0S_STATE_TOTAL_GOTTEN,
	EPSONISTORE0S_STATE_PAYMENT_TYPE_GOTTEN,
	EPSONISTORE0S_STATE_CHANGE_GOTTEN,
	EPSONISTORE0S_STATE_THANK_GOTTEN,
	EPSONISTORE0S_STATE_COMPLETION_START_GOTTEN,
	EPSONISTORE0S_STATE_COMPLETION_END_GOTTEN = EPSONISTORE0S_STATE_BEGINNING,
	EPSONISTORE0S_STATE_TRANSACTION_VOIDED_GOTTEN = EPSONISTORE0S_STATE_BEGINNING,
};

enum EPSONISTORE0S_ITEM_TYPE
{
	EPSONISTORE0S_NULL_ITEM,
	EPSONISTORE0S_HEAD_ITEM,
	EPSONISTORE0S_WHOLE_ITEM,
};

/*
* the parser class for EpsonIStore0S POS sale transaction data.		
*/
class CEpsonIStore0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CEpsonIStore0SParser( CReceiver *receiver );

	virtual ~CEpsonIStore0SParser();

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
	*				item type.
	*/
	EPSONISTORE0S_ITEM_TYPE setItem( char *buffer );

	/*
	* Function:		
	*				look for quantity and amount of sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemMore( char *buffer );

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
	*				look for subtotal information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setSubtotal( char *buffer );

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
	*				look for change information.
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

	EPSONISTORE0S_PARSE_STATE m_state;

	char			m_itemName[48];
	
};

#endif // !defined(AFX_EPSONISTORE0S_H__ED942885_4F7F_45FC_9EEB_7F4B995F77BD__INCLUDED_)
