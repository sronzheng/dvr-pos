// EpsonCR0SParser.h: interface for the CEpsonCR0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPSONCR0SPARSER_H__E64A97AF_0DCA_43D9_8C2E_ACDE5D4097CB__INCLUDED_)
#define AFX_EPSONCR0SPARSER_H__E64A97AF_0DCA_43D9_8C2E_ACDE5D4097CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Parser.h"

/*
* enum type for states of the CEpsonCR0SParser.		
*/
enum EPSONCR0S_PARSE_STATE
{
	EPSONCR0S_STATE_BEGINNING,
	EPSONCR0S_STATE_DATE_TIME_GOTTEN,
	EPSONCR0S_STATE_SALE_ITEM_HEAD_GOTTEN,
	EPSONCR0S_STATE_SALE_ITEM_GOTTEN,
	EPSONCR0S_STATE_PAYMENT_TYPE_GOTTEN,
	EPSONCR0S_STATE_SUBTOTAL_GOTTEN,
	EPSONCR0S_STATE_TAX_GOTTEN,
	EPSONCR0S_STATE_TOTAL_GOTTEN,
	EPSONCR0S_STATE_CHANGE_GOTTEN,
	EPSONCR0S_STATE_NO_SALE_GOTTEN,
	EPSONCR0S_STATE_THANK_GOTTEN = EPSONCR0S_STATE_BEGINNING,
};

/*
* the parser class for EpsonCR0S POS sale transaction data.		
*/
class CEpsonCR0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CEpsonCR0SParser( CReceiver *receiver );

	virtual ~CEpsonCR0SParser();

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
	*				look for the head of sale item, 
	*               including quantiy and unit price.
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
	*				look for change information
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

	EPSONCR0S_PARSE_STATE m_state;

private:

	bool			m_itemHeadGotten;
	char			m_itemQuantity;
	MONEY_AMOUNT	m_itemUnitPrice;
};

#endif // !defined(AFX_EPSONCR0SPARSER_H__E64A97AF_0DCA_43D9_8C2E_ACDE5D4097CB__INCLUDED_)
