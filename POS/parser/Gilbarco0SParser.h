// Gilbarco0SParser.h: interface for the CGilbarco0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GILBARCO0SPARSER_H__FF86D1D0_C261_47B9_A9A0_1EEA6C9AE896__INCLUDED_)
#define AFX_GILBARCO0SPARSER_H__FF86D1D0_C261_47B9_A9A0_1EEA6C9AE896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

enum GILBARCO0S_PARSE_STATE
{
	GILBARCO0S_STATE_BEGINNING,
	GILBARCO0S_STATE_SALE_ITEM_HEAD_GOTTEN,
	GILBARCO0S_STATE_SALE_ITEM_GOTTEN,
	GILBARCO0S_STATE_SUBTOTAL_GOTTEN,
	GILBARCO0S_STATE_TAX_GOTTEN,
	GILBARCO0S_STATE_TOTAL_GOTTEN,
	GILBARCO0S_STATE_PAYMENT_TYPE_GOTTEN,
	GILBARCO0S_STATE_PAY_IN_GOTTEN,
	GILBARCO0S_STATE_CHANGE_GOTTEN = GILBARCO0S_STATE_BEGINNING,
	GILBARCO0S_STATE_CASH_GOTTEN = GILBARCO0S_STATE_BEGINNING,
};

enum GILBARCO0S_ITEM_TYPE
{
	GILBARCO0S_NULL_ITEM,
	GILBARCO0S_HEAD_ITEM,
	GILBARCO0S_MAIN_ITEM,
};

class CGilbarco0SParser : public CParser   
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CGilbarco0SParser( CReceiver *receiver );

	virtual ~CGilbarco0SParser();

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
	*				look for sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	GILBARCO0S_ITEM_TYPE setItem( char *buffer );

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
	*				look for change information
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setChange( char *buffer );

	GILBARCO0S_PARSE_STATE m_state;

	bool m_transactionInformed;

private:

	bool			m_itemHeadGotten;
	int				m_itemQuantity;
	MONEY_AMOUNT	m_itemUnitPrice;
};

#endif // !defined(AFX_GILBARCO0SPARSER_H__FF86D1D0_C261_47B9_A9A0_1EEA6C9AE896__INCLUDED_)
