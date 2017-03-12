// Casio61000SParser.h: interface for the CCasio61000SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASIO61000SPARSER_H__ADA5684D_1207_4B40_86F0_E2D36D0F3609__INCLUDED_)
#define AFX_CASIO61000SPARSER_H__ADA5684D_1207_4B40_86F0_E2D36D0F3609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CCasio61000SParser.		
*/
enum CASIO61000S_PARSE_STATE
{
	CASIO61000S_STATE_BEGINNING,
	CASIO61000S_STATE_DATE_TIME_GOTTEN,
	CASIO61000S_STATE_SALE_ITEM_GOTTEN,
	CASIO61000S_STATE_TOTAL_GOTTEN,
	CASIO61000S_STATE_PAYMENT_TYPE_GOTTEN = CASIO61000S_STATE_BEGINNING,
};

/*
* the parser class for Casio61000S POS sale transaction data.		
*/
class CCasio61000SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CCasio61000SParser( CReceiver *receiver );

	virtual ~CCasio61000SParser();

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

	CASIO61000S_PARSE_STATE m_state;

};

#endif // !defined(AFX_CASIO61000SPARSER_H__ADA5684D_1207_4B40_86F0_E2D36D0F3609__INCLUDED_)
