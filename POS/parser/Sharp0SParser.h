// Sharp0SParser.h: interface for the CSharp0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHARP0SPARSER_H__58AF7AC2_9FBB_4B76_BB22_73597A911DCB__INCLUDED_)
#define AFX_SHARP0SPARSER_H__58AF7AC2_9FBB_4B76_BB22_73597A911DCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

/*
* enum type for states of the CSharp0SParser.		
*/
enum SHARP0S_PARSE_STATE
{
	SHARP0S_STATE_BEGINNING,
	SHARP0S_STATE_DATE_GOTTEN,
	SHARP0S_STATE_TIME_GOTTEN,
	SHARP0S_STATE_SALE_ITEM_GOTTEN,
	SHARP0S_STATE_SUBTOTAL_GOTTEN,
	SHARP0S_STATE_TAX_GOTTEN,
	SHARP0S_STATE_TOTAL_GOTTEN,
	SHARP0S_STATE_PAYMENT_TYPE_GOTTEN,
	SHARP0S_STATE_CHANGE_GOTTEN = SHARP0S_STATE_BEGINNING,
	SHARP0S_STATE_NO_SALE_GOTTEN = SHARP0S_STATE_BEGINNING,
};

class CSharp0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CSharp0SParser( CReceiver *receiver );

	virtual ~CSharp0SParser();

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
	*				look for data information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setDate( char *buffer );

	/*
	* Function:		
	*				look for time information.
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

	SHARP0S_PARSE_STATE m_state;

private:

	int m_year, m_month, m_day;

};

#endif // !defined(AFX_SHARP0SPARSER_H__58AF7AC2_9FBB_4B76_BB22_73597A911DCB__INCLUDED_)
