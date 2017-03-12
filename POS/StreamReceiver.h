// StreamReceiver.h: interface for the CStreamReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMRECEIVER_H__265EEC29_557C_4A7D_B3F4_D28C196C45EC__INCLUDED_)
#define AFX_STREAMRECEIVER_H__265EEC29_557C_4A7D_B3F4_D28C196C45EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Receiver.h"
#include "binarystream.h"

/*
* Receive pos transaction information ,
* and save into a binary data stream.
*/
class CStreamReceiver : public CReceiver  
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
	*	stream:		the data binary stream to write into.
	*/
	CStreamReceiver( CBinaryStream &stream );

	virtual ~CStreamReceiver();

private:
	CBinaryStream &m_stream;
};

#endif // !defined(AFX_STREAMRECEIVER_H__265EEC29_557C_4A7D_B3F4_D28C196C45EC__INCLUDED_)
