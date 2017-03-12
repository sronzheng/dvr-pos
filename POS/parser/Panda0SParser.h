// Panda0SParser.h: interface for the CPanda0SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANDA0SPARSER_H__68E4905E_A047_4316_ADB1_A1680706422D__INCLUDED_)
#define AFX_PANDA0SPARSER_H__68E4905E_A047_4316_ADB1_A1680706422D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

enum PANDA0S_PARSE_STATE
{
	PANDA0S_STATE_BEGINNING,
	PANDA0S_STATE_REGISTER_GOTTEN,
	PANDA0S_STATE_SALE_ITEM_GOTTEN,
	PANDA0S_STATE_TAX_GOTTEN,
	PANDA0S_STATE_TOTAL_GOTTEN,
	PANDA0S_STATE_PAYMENT_TYPE_GOTTEN,
	PANDA0S_STATE_CHANGE_GOTTEN,
	PANDA0S_STATE_DATE_TIME_GOTTEN = PANDA0S_STATE_BEGINNING,
};

class CPanda0SParser : public CParser   
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/	
	CPanda0SParser( CReceiver *receiver );

	virtual ~CPanda0SParser();

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
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTime( char *buffer, bool inform = true );

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

	PANDA0S_PARSE_STATE m_state;

	bool m_transactionInformed;

};

#endif // !defined(AFX_PANDA0SPARSER_H__68E4905E_A047_4316_ADB1_A1680706422D__INCLUDED_)
