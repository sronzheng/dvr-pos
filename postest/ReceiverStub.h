// StdOutputFake.h: interface for the CReceiverStub class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDOUTPUTFAKE_H__E92F7118_F781_45D8_8BB8_FF559F4F89CC__INCLUDED_)
#define AFX_STDOUTPUTFAKE_H__E92F7118_F781_45D8_8BB8_FF559F4F89CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "receiver.h"

class CReceiverStub : public CReceiver  
{
public:

	virtual void setTransaction();

	virtual void setTime( int year, 
						  int month, 
						  int day, 
						  int hour, 
						  int minute );

	virtual void setCardType( char typeName[] );

	virtual void setItem( char name[],
						  const MONEY_AMOUNT &unitPrice,
						  int quantity,					  
						  const MONEY_AMOUNT &price ); 

	virtual void setItemVoided( const MONEY_AMOUNT &amount ); 

	virtual void setSubtotal( const MONEY_AMOUNT &subtotal );

	virtual void setTax( const MONEY_AMOUNT &tax );

	virtual void setTotal( const MONEY_AMOUNT &total );

	virtual void setPaymentType( PAYMENT_TYPE type, 
								 const MONEY_AMOUNT &amount );

	virtual void setChange( const MONEY_AMOUNT &change );

	virtual void setRefund( const MONEY_AMOUNT &amount );

	virtual void setTransactionVoided();

	CReceiverStub();
	~CReceiverStub();

	int m_CountOfDateTime;

	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;

	MONEY_AMOUNT m_amountVoided;

	int m_CountOfSetItem;

	std::string m_itemName;
	MONEY_AMOUNT m_unitAmount;
	int m_itemQuantity;
	MONEY_AMOUNT m_itemAmount;

	std::string m_cardType;

	MONEY_AMOUNT m_subtotal;

	MONEY_AMOUNT m_tax;

	MONEY_AMOUNT m_total;

	int m_CountOfPayment;

	PAYMENT_TYPE m_paymentType;
	MONEY_AMOUNT m_paymentAmount;

	MONEY_AMOUNT m_change;

	MONEY_AMOUNT m_refund;

	bool m_transactionVoided;

	int m_countOfTransactions;
};

#endif // !defined(AFX_STDOUTPUTFAKE_H__E92F7118_F781_45D8_8BB8_FF559F4F89CC__INCLUDED_)
