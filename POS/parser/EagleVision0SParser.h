// CEagleVision0SParser.h: interface for the CCasioParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EAGLEVISION0SPARSER_H__8C6914F3_51C4_49C5_9E8B_F08238C12C32__INCLUDED_)
#define AFX_EAGLEVISION0SPARSER_H__8C6914F3_51C4_49C5_9E8B_F08238C12C32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CEagleVision0SParser.		
*/
enum EAGLEVISION0S_PARSE_STATE
{
	EAGLEVISION0S_STATE_BEGINNING,
	EAGLEVISION0S_STATE_DATE_TIME_GOTTEN,
	EAGLEVISION0S_STATE_SALE_ITEM_HEAD_GOTTEN,
	EAGLEVISION0S_STATE_SALE_ITEM_GOTTEN,
	EAGLEVISION0S_STATE_SUBTOTAL_GOTTEN,
	EAGLEVISION0S_STATE_TOTAL_GOTTEN,
	EAGLEVISION0S_STATE_PAYMENT_TYPE_GOTTEN,
	EAGLEVISION0S_STATE_REFUND_GOTTEN,
	EAGLEVISION0S_STATE_TRANSACTION_VOIDED_GOTTEN,
	EAGLEVISION0S_STATE_CARD_TYPE_GOTTEN,
	EAGLEVISION0S_STATE_END_FLAG_GOTTEN = EAGLEVISION0S_STATE_BEGINNING,
};

/*
* the parser class for EagleVision0S POS sale transaction data.		
*/
class CEagleVision0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CEagleVision0SParser( CReceiver *receiver );

	virtual ~CEagleVision0SParser();

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
	*				look for the head of sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemHead( char *buffer );

	/*
	* Function:		
	*				look for the end of sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemEnd( char *buffer );

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
	*				look for payment by cash.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setCashOfPayment( char *buffer );

	/*
	* Function:		
	*				look for payment type. it support payment by cash via 
	*				setCashOfPayment().
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setPaymentType( char *buffer );

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
	*				look for refund information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setRefund( char *buffer );

	/*
	* Function:		
	*				look for transaction voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTransactionVoided( char *buffer );

	EAGLEVISION0S_PARSE_STATE m_state;

private:

	int				m_itemQuantity;
	MONEY_AMOUNT	m_itemUnitPrice;
};

#endif // !defined(AFX_CASIOPARSER_H__8C6914F3_51C4_49C5_9E8B_F08238C12C32__INCLUDED_)
