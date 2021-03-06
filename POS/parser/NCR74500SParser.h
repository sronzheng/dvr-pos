// NCR74500SParser.h: interface for the CNCR74500SParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCR74500SPARSER_H__8AFCF1B1_7BC7_4C4C_93AC_50B17808672B__INCLUDED_)
#define AFX_NCR74500SPARSER_H__8AFCF1B1_7BC7_4C4C_93AC_50B17808672B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

/*
* enum type for states of the CNCR74500SParser.		
*/
enum NCR74500S_PARSE_STATE
{
	NCR74500S_STATE_BEGINNING,
	NCR74500S_STATE_DATE_TIME_GOTTEN,
	NCR74500S_STATE_WELCOME_GOTTEN,
	NCR74500S_STATE_SALE_ITEM_GOTTEN,
	NCR74500S_STATE_SUBTOTAL_GOTTEN,
	NCR74500S_STATE_TAX_GOTTEN,
	NCR74500S_STATE_TOTAL_GOTTEN,
	NCR74500S_STATE_PAYMENT_TYPE_GOTTEN,
	NCR74500S_STATE_CHANGE_GOTTEN = NCR74500S_STATE_BEGINNING,
	NCR74500S_STATE_TRANSACTION_VOIDED_GOTTEN = NCR74500S_STATE_BEGINNING,
	NCR74500S_STATE_NO_SALE_GOTTEN = NCR74500S_STATE_BEGINNING,
	NCR74500S_STATE_SAFE_DROP_GOTTEN = NCR74500S_STATE_BEGINNING,
};

/*
* the parser class for EagleVision0S POS sale transaction data.		
*/
class CNCR74500SParser : public CParser  
{
public:

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	receiver:	pointer to receiver object.
	*/
	CNCR74500SParser( CReceiver *receiver );

	virtual ~CNCR74500SParser();

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

	/*
	* Function:		
	*				look for transaction voided.
	* Parameters:
	*	buffer:		pointer to buffer to be parsed.
	* Return Value:
	*				true if found, false if not.
	*/
	bool setTransactionVoided( char *buffer );

	NCR74500S_PARSE_STATE m_state;
};

#endif // !defined(AFX_NCR74500SPARSER_H__8AFCF1B1_7BC7_4C4C_93AC_50B17808672B__INCLUDED_)
