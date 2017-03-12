// EpsonHRK_ZA0SParser.h: interface for the CEpsonHRK_ZA0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPSONHRK_ZA0SPARSER_H__E34230EA_B24D_4129_910C_76A6350595EF__INCLUDED_)
#define AFX_EPSONHRK_ZA0SPARSER_H__E34230EA_B24D_4129_910C_76A6350595EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CEpsonHRK_ZA0SParser.		
*/
enum EPSONHRK_ZA0S_PARSE_STATE
{
	EPSONHRK_ZA0S_STATE_BEGINNING,
	EPSONHRK_ZA0S_STATE_WELCOME_GOTTEN,
	EPSONHRK_ZA0S_STATE_SALE_ITEM_GOTTEN,
	EPSONHRK_ZA0S_STATE_TOTAL_GOTTEN,
	EPSONHRK_ZA0S_STATE_PAYMENT_TYPE_GOTTEN,
	EPSONHRK_ZA0S_STATE_TRANSACTION_VOIDED_GOTTEN,
	EPSONHRK_ZA0S_STATE_PAID_OUT_GOTTEN,
	EPSONHRK_ZA0S_STATE_DATE_TIME_GOTTEN = EPSONHRK_ZA0S_STATE_BEGINNING,
};

/*
* the parser class for EpsonHRK-ZA0S POS sale transaction data.		
*/
class CEpsonHRK_ZA0SParser : public CParser 
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CEpsonHRK_ZA0SParser( CReceiver *receiver );

	virtual ~CEpsonHRK_ZA0SParser();

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
	*				look for welcome message.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setWelcome( char *buffer );

	/*
	* Function:		
	*				look for date&time information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
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

	EPSONHRK_ZA0S_PARSE_STATE m_state;

};

#endif // !defined(AFX_EPSONHRK_ZA0SPARSER_H__E34230EA_B24D_4129_910C_76A6350595EF__INCLUDED_)
