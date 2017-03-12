// WndReceiver.h: interface for the CWndReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDRECEIVER_H__30071E74_1355_4D83_BD18_E97AF1B3F4C8__INCLUDED_)
#define AFX_WNDRECEIVER_H__30071E74_1355_4D83_BD18_E97AF1B3F4C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Receiver.h"

/*
* Receive pos transaction information,
* and display on a window.
*/
class CWndReceiver : public CReceiver  
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
						  const MONEY_AMOUNT &price ); 

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
	*	view:		the window to display on.
	*/
	CWndReceiver( CListBox *view );

	virtual ~CWndReceiver();

private:
	CListBox *m_view;
};

#endif // !defined(AFX_WNDRECEIVER_H__30071E74_1355_4D83_BD18_E97AF1B3F4C8__INCLUDED_)
