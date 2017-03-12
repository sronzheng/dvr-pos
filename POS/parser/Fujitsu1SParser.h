// Fujitsu1SParser.h: interface for the CFujitsu1SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUJITSU1SPARSER_H__CEDC3B89_626D_4347_A6E8_A4678ABF9EF7__INCLUDED_)
#define AFX_FUJITSU1SPARSER_H__CEDC3B89_626D_4347_A6E8_A4678ABF9EF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CFujitsu1SParser.		
*/
enum FUJITSU1S_PARSE_STATE
{
	FUJITSU1S_STATE_BEGINNING,
	FUJITSU1S_STATE_DATE_TIME_EXPECTED = FUJITSU1S_STATE_BEGINNING,
	FUJITSU1S_STATE_DATE_TIME_GOTTEN,
	FUJITSU1S_STATE_SALE_ITEM_GOTTEN,
	FUJITSU1S_STATE_PAYMENT_TYPE_GOTTEN,
	FUJITSU1S_STATE_SUBTOTAL_GOTTEN,
	FUJITSU1S_STATE_TAX_GOTTEN,
	FUJITSU1S_STATE_TOTAL_GOTTEN,
	FUJITSU1S_STATE_CHANGE_GOTTEN,
	FUJITSU1S_STATE_TRANSACTION_VOIDED_GOTTEN,
	FUJITSU1S_STATE_SAFE_LOAN_GOTTEN,
	FUJITSU1S_STATE_SAFE_DROP_GOTTEN,
	FUJITSU1S_STATE_PAID_OUT_GOTTEN,
	FUJITSU1S_STATE_END_FLAG_GOTTEN = FUJITSU1S_STATE_BEGINNING,
};

/*
* the parser class for Fujitsu1S POS sale transaction data.		
*/
class CFujitsu1SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CFujitsu1SParser( CReceiver *receiver );
	
	virtual ~CFujitsu1SParser();

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
	*				look for change information
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setChange( char *buffer );

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

	/*
	* Function:		
	*				do some cleanup work.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setEndFlag( char *buffer );

	FUJITSU1S_PARSE_STATE m_state;

	bool			m_timeInformed;

private:

	int				m_year;
	int				m_month;
	int				m_day;
	int				m_hour;
	int				m_minute;
};

#endif // !defined(AFX_FUJITSU1SPARSER_H__CEDC3B89_626D_4347_A6E8_A4678ABF9EF7__INCLUDED_)
