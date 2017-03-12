// RubyCR0SParser.h: interface for the CRubyCR0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUBYCR0SPARSER_H__9A3BB324_7EFA_4E9A_BF7A_15DDCEE81ED1__INCLUDED_)
#define AFX_RUBYCR0SPARSER_H__9A3BB324_7EFA_4E9A_BF7A_15DDCEE81ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

enum RUBYCR0S_PARSE_STATE
{
	RUBYCR0S_STATE_BEGINNING,
	RUBYCR0S_STATE_SAFE_LOAN_GOTTEN,
	RUBYCR0S_STATE_PAY_OUT_GOTTEN,
	RUBYCR0S_STATE_ERROR_HEAD_GOTTEN,
	RUBYCR0S_STATE_ERROR_TAIL_GOTTEN,
	RUBYCR0S_STATE_SALE_ITEM_GOTTEN,
	RUBYCR0S_STATE_SUBTOTAL_GOTTEN,
	RUBYCR0S_STATE_TAX_GOTTEN,
	RUBYCR0S_STATE_TOTAL_GOTTEN,
	RUBYCR0S_STATE_PAYMENT_TYPE_GOTTEN,
	RUBYCR0S_STATE_CHANGE_GOTTEN,
	RUBYCR0S_STATE_FIRST_DATE_TIME_GOTTEN,
	RUBYCR0S_STATE_MORE_DATE_TIME_GOTTEN,
	RUBYCR0S_STATE_THANK_GOTTEN,
};

class CRubyCR0SParser : public CParser 
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CRubyCR0SParser( CReceiver *receiver );

	virtual ~CRubyCR0SParser();

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
	*	inform:		indicate whether inform receiver object, 
	*				when time item found.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTime( char *buffer , bool inform = true );

	/*
	* Function:		
	*				look for discount item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setProItem( char *buffer );

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
	*				look for change information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setChange( char *buffer );

	RUBYCR0S_PARSE_STATE m_state;

	bool m_transactionInformed;

private:

	char			m_preItemName[48];
	MONEY_AMOUNT	m_preItemAmount;
	MONEY_AMOUNT	m_preProAmount;

	PAYMENT_TYPE	m_prePaymentType;
	MONEY_AMOUNT	m_prePaymentAmount;
};

#endif // !defined(AFX_RUBYCR0SPARSER_H__9A3BB324_7EFA_4E9A_BF7A_15DDCEE81ED1__INCLUDED_)
