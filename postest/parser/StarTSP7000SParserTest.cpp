// StarTSP7000SParserTest.cpp: implementation of the CStarTSP7000SParserTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StarTSP7000SParserTest.h"

#include "StarTSP7000SParser.h"

#include "receiverstub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CStarTSP7000SParserTest );

CStarTSP7000SParserTest::CStarTSP7000SParserTest()
{

}

CStarTSP7000SParserTest::~CStarTSP7000SParserTest()
{

}

void CStarTSP7000SParserTest::testSetTime1()
{
	CReceiverStub receiver;

	CStarTSP7000SParser parser( &receiver );

	// data & time
	// 12-13-2005, 14:15
	char buf[] = "Datum: 13.12.2005 Ura:  14:15";

	CPPUNIT_ASSERT( parser.setTime( buf ) );

	CPPUNIT_ASSERT( receiver.m_countOfTransactions == 1 );

	CPPUNIT_ASSERT( receiver.m_year == 2005 );
	CPPUNIT_ASSERT( receiver.m_month == 12 );
	CPPUNIT_ASSERT( receiver.m_day == 13 );
	CPPUNIT_ASSERT( receiver.m_hour == 14 );
	CPPUNIT_ASSERT( receiver.m_minute == 15 );
}

void CStarTSP7000SParserTest::testSetItem1() 
{
	CReceiverStub receiver;

	CStarTSP7000SParser parser( &receiver );

	// sale item. 
	// quantity:	3,				unit price:	170.00 dollar
    // name: WRIG ALPINE CH EACH,	amount:		510.00 dollar  

	char buf1[] = "KAVA Z MLEKOM         3.00 X      170.00";
	char buf2[] = "                                  510.00";

	CPPUNIT_ASSERT( parser.setItemHead( buf1 ) );
	CPPUNIT_ASSERT( parser.setItemEnd( buf2 ) );

	CPPUNIT_ASSERT( receiver.m_itemName.compare( "KAVA Z MLEKOM" ) == 0 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 170 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemQuantity == 3 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 510 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );
}

void CStarTSP7000SParserTest::testSetTotal1() 
{
	CReceiverStub receiver;

	CStarTSP7000SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "Skupaj:     " "\x1B" "i11          390.01" "\x1B" "i00";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 390 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 1 );
}

void CStarTSP7000SParserTest::testSetTotal2() 
{
	CReceiverStub receiver;

	CStarTSP7000SParser parser( &receiver );

	// total. 
	// total:	5.82 dollar  
	char buf[] = "Skupaj:     " "\x1B" "i11      1,992,390.01" "\x1B" "i00";

	CPPUNIT_ASSERT( parser.setTotal( buf ) );

	CPPUNIT_ASSERT( receiver.m_total.dollar == 1992390 );
	CPPUNIT_ASSERT( receiver.m_total.cent == 1 );
}

void CStarTSP7000SParserTest::testActualParse1()
{
	// state transform.

	char buf1[] = "Datum: 22.12.2004 Ura:  8:39";				// Time Gotten		
	char buf2[] = "KAKAO                 1.00 X      200.00";
	char buf3[] = "                                  200.00";	// Item Gotten
	char buf4[] = "PEPSI STEKL.          3.00 X      250.00";
	char buf5[] = "                                  750.00";	// Item Gotten
	char buf6[] = "Skupaj: " "\x1B" "i11   950.00" "\x1B" "i00";// Total Gotten

	CReceiverStub receiver;

	CStarTSP7000SParser parser( &receiver );

	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_BEGINNING );

	parser.parseLine( buf1 );
	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_DATE_TIME_GOTTEN );

	parser.parseLine( buf2 );
	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf3 );
	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_SALE_ITEM_END_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 200 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 200 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );

	parser.parseLine( buf4 );
	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_SALE_ITEM_HEAD_GOTTEN );

	parser.parseLine( buf5 );
	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_SALE_ITEM_END_GOTTEN );
	CPPUNIT_ASSERT( receiver.m_unitAmount.dollar == 250 );
	CPPUNIT_ASSERT( receiver.m_unitAmount.cent == 0 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.dollar == 750 );
	CPPUNIT_ASSERT( receiver.m_itemAmount.cent == 0 );

	parser.parseLine( buf6 );
	CPPUNIT_ASSERT( parser.m_state == STARTSP7000S_STATE_TOTAL_GOTTEN );
}
