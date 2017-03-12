// Casio6100A0SParser.h: interface for the CCasio6100A0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASIO6100A0SPARSER_H__9A968110_13B4_4E80_ABB1_29E282349816__INCLUDED_)
#define AFX_CASIO6100A0SPARSER_H__9A968110_13B4_4E80_ABB1_29E282349816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CCasio6100A0SParser.		
*/
enum CASIO6100A0S_PARSE_STATE
{
	CASIO6100A0S_STATE_BEGINNING,
	CASIO6100A0S_STATE_DATE_TIME_GOTTEN,
	CASIO6100A0S_STATE_SALE_ITEM_GOTTEN,
	CASIO6100A0S_STATE_TOTAL_GOTTEN,
	CASIO6100A0S_STATE_PAYMENT_TYPE_GOTTEN,
	CASIO6100A0S_STATE_CHANGE_GOTTEN = CASIO6100A0S_STATE_BEGINNING,
};

/*
* the parser class for Casio6100A0S POS sale transaction data.		
*/
class CCasio6100A0SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CCasio6100A0SParser( CReceiver *receiver );

	virtual ~CCasio6100A0SParser();

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

	/*
	* Function:		
	*				look for change. 
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setChange( char *buffer );

	CASIO6100A0S_PARSE_STATE m_state;

};

#endif // !defined(AFX_CASIO6100A0SPARSER_H__9A968110_13B4_4E80_ABB1_29E282349816__INCLUDED_)
