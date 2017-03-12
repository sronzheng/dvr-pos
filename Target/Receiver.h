// Receiver.h: interface for the CReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECEIVER_H__78A0AB01_D99C_4612_AF80_293B7F7075BA__INCLUDED_)
#define AFX_RECEIVER_H__78A0AB01_D99C_4612_AF80_293B7F7075BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
* money definition struct		
*/
typedef struct _MONEY_AMOUNT
{
	int	dollar;			// dollar part
	int cent;			// cent part
} MONEY_AMOUNT;

/*
* payment type. 
*/
enum PAYMENT_TYPE
{
	CASH,					// by cash
	CHECK,					// by check
	CREDIT,					// by credit card
	ATM,					// by ATM card
	GIFT_CERTIFICATE		// by gift certificate	
};

/*
* interface to specify the POS sale transaction information receiver.		
*/
class CReceiver  
{
public:

	/*
	* Function:		
	*				be informed that the transaction is voided.
	*/
	virtual void setTransaction() = 0;

	/*
	* Function:		
	*				date and time information informed.
	* Parameters:
	*	year:		year of date
	*   month:		month of date
	*	day:		day of date
	*	hour:		hour of time
	*	minute:		minute of time
	*/
	virtual void setTime( int year, 
						  int month, 
						  int day, 
						  int hour, 
						  int minute ) = 0;

	/*
	* Function:		
	*				card type information informed.
	* Parameters:
	*	typeName:	name of card type
	*/
	virtual void setCardType( char typeName[] ) = 0;

	/*
	* Function:		
	*				sale item information informed.
	* Parameters:
	*	quantity:		quantity of sale item
	*	name:		name of sale item
	*	price:		price of sale item
	*/
	virtual void setItem( char name[],
						  const MONEY_AMOUNT &unitPrice,
						  int quantity,					  
						  const MONEY_AMOUNT &price ) = 0; 

	/*
	* Function:		
	*				be informed that a sale item is voided.
	* Parameters:
	*	number:		number of sale item
	*	name:		name of sale item
	*	price:		price of sale item
	*/
	virtual void setItemVoided( const MONEY_AMOUNT &amount ) = 0; 

	/*
	* Function:		
	*				subtotal information of the transaction informed. 
	* Parameters:
	*	amount:	subtotal amount of the transaction
	*/
	virtual void setSubtotal( const MONEY_AMOUNT &amount ) = 0;

	/*
	* Function:		
	*				tax information of the transaction informed. 
	* Parameters:
	*	amount:		tax of the transaction
	*/
	virtual void setTax( const MONEY_AMOUNT &amount ) = 0;

	/*
	* Function:		
	*				total information of the transaction informed. 
	* Parameters:
	*	amount:		total amount of the transaction 
	*/
	virtual void setTotal( const MONEY_AMOUNT &amount ) = 0;

	/*
	* Function:		
	*				payment type informed.
	* Parameters:
	*	type:		payment type.
	*	amount:		amount of the transaction.
	*/
	virtual void setPaymentType( PAYMENT_TYPE type, 
								 const MONEY_AMOUNT &amount ) = 0;

	/*
	* Function:		
	*				change information informed.
	* Parameters:
	*	amount:		change amount of the transaction.
	*/
	virtual void setChange( const MONEY_AMOUNT &amount ) = 0;

	/*
	* Function:		
	*				refund information informed.
	* Parameters:
	*	amount:		amount of refund.
	*/
	virtual void setRefund( const MONEY_AMOUNT &amount ) = 0;

	/*
	* Function:		
	*				be informed that the transaction is voided.
	*/
	virtual void setTransactionVoided() = 0;

	CReceiver();
	
	virtual ~CReceiver();

};

#endif // !defined(AFX_RECEIVER_H__78A0AB01_D99C_4612_AF80_293B7F7075BA__INCLUDED_)
