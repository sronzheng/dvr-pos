// Sams40SParser.h: interface for the CSams40SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMS40SPARSER_H__29EAC657_745D_429C_B8DA_28F45AF0D0AE__INCLUDED_)
#define AFX_SAMS40SPARSER_H__29EAC657_745D_429C_B8DA_28F45AF0D0AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

/*
* enum type for states of the CSams40SParser.		
*/
enum SAMS40S_PARSE_STATE
{
	SAMS40S_STATE_BEGINNING,
	SAMS40S_STATE_DATE_TIME_GOTTEN,
	SAMS40S_STATE_SALE_ITEM_GOTTEN,
	SAMS40S_STATE_TOTAL_GOTTEN,
	SAMS40S_STATE_PAYMENT_TYPE_GOTTEN,
	SAMS40S_STATE_CHANGE_GOTTEN = SAMS40S_STATE_BEGINNING,
};

/*
* the parser class for Sams40S POS sale transaction data.		
*/
class CSams40SParser : public CParser  
{
public:
	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CSams40SParser( CReceiver *receiver );

	virtual ~CSams40SParser();

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

	SAMS40S_PARSE_STATE m_state;

};

#endif // !defined(AFX_SAMS40SPARSER_H__29EAC657_745D_429C_B8DA_28F45AF0D0AE__INCLUDED_)
