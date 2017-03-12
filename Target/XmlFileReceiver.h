// XmlFileReceiver.h: interface for the CXmlFileReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLFILERECEIVER_H__0656F8B5_FC91_451A_AE85_78671FF60810__INCLUDED_)
#define AFX_XMLFILERECEIVER_H__0656F8B5_FC91_451A_AE85_78671FF60810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <fstream>

#include <stack>

#include "Receiver.h"

enum ITEM_LEVEL
{
	TOP,
	TRANSACTION,
	ITEM,
	CHILD_ITEM
};

typedef struct _BACK_LEVEL
{
	char		back[36];
	ITEM_LEVEL	level;
} BACK_LEVEL;

/*
* Receive pos transaction information,
* and save into a file of xml format.
*/
class CXmlFileReceiver : public CReceiver  
{
public:

	/*
	* Refer to CReceiver
	*/
	virtual void setTransaction();

	/*
	* Refer to CReceiver
	*/
	virtual void setTime( int year, 
						  int month, 
						  int day, 
						  int hour, 
						  int minute );

	/*
	* Refer to CReceiver
	*/
	virtual void setCardType( char typeName[] );

	/*
	* Refer to CReceiver
	*/
	virtual void setItem( char name[],
						  const MONEY_AMOUNT &unitPrice,
						  int quantity,					  
						  const MONEY_AMOUNT &pricee ); 

	/*
	* Refer to CReceiver
	*/
	virtual void setItemVoided( const MONEY_AMOUNT &amount ); 

	/*
	* Refer to CReceiver
	*/
	virtual void setSubtotal( const MONEY_AMOUNT &amount );

	/*
	* Refer to CReceiver
	*/
	virtual void setTax( const MONEY_AMOUNT &amount );

	/*
	* Refer to CReceiver
	*/
	virtual void setTotal( const MONEY_AMOUNT &amount );

	/*
	* Refer to CReceiver
	*/
	virtual void setPaymentType( PAYMENT_TYPE type, 
								 const MONEY_AMOUNT &amount );

	/*
	* Refer to CReceiver
	*/
	virtual void setChange( const MONEY_AMOUNT &amount );

	/*
	* Refer to CReceiver
	*/
	virtual void setRefund( const MONEY_AMOUNT &amount );

	/*
	* Refer to CReceiver
	*/
	virtual void setTransactionVoided();

	/*
	* Function:		
	*				constructor.
	* Parameters:
	*	of:			output stream ( xml file ).
	*/
	CXmlFileReceiver( std::ofstream& of );

	virtual ~CXmlFileReceiver();

private:

	void PopLowerLevelBack( ITEM_LEVEL curLevel);

	std::ofstream &m_of;

	std::stack<BACK_LEVEL *> m_backs;
};

#endif // !defined(AFX_XMLFILERECEIVER_H__0656F8B5_FC91_451A_AE85_78671FF60810__INCLUDED_)
