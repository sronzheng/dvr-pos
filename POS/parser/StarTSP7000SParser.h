// StarTSP7000SParser.h: interface for the CStarTSP7000SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STARTSP7000SPARSER_H__D7B9CE3D_D4B5_43E9_96C4_0F1A4609D08C__INCLUDED_)
#define AFX_STARTSP7000SPARSER_H__D7B9CE3D_D4B5_43E9_96C4_0F1A4609D08C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"

/*
* enum type for states of the CStarTSP7000SParser.		
*/
enum STARTSP7000S_PARSE_STATE
{
	STARTSP7000S_STATE_BEGINNING,
	STARTSP7000S_STATE_DATE_TIME_GOTTEN,
	STARTSP7000S_STATE_SALE_ITEM_HEAD_GOTTEN,
	STARTSP7000S_STATE_SALE_ITEM_END_GOTTEN,
	STARTSP7000S_STATE_TOTAL_GOTTEN = STARTSP7000S_STATE_BEGINNING,
};

/*
* the parser class for StarTSP7000S POS sale transaction data.		
*/
class CStarTSP7000SParser : public CParser  
{
public:


	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CStarTSP7000SParser( CReceiver *receiver );

	virtual ~CStarTSP7000SParser();

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
	*				look for the head line of sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemHead( char *buffer );

	/*
	* Function:		
	*				look for the end line of sale item.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setItemEnd( char *buffer );

	/*
	* Function:		
	*				look for total information.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTotal( char *buffer );

	STARTSP7000S_PARSE_STATE m_state;

private:

	int				m_itemQuantity;
	MONEY_AMOUNT	m_itemUnitPrice;
	char			m_itemName[48];

};

#endif // !defined(AFX_STARTSP7000SPARSER_H__D7B9CE3D_D4B5_43E9_96C4_0F1A4609D08C__INCLUDED_)
