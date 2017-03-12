// ParserFactoryTest.cpp: implementation of the CParserFactoryTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParserFactoryTest.h"

#include "parserfactory.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CParserFactoryTest );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParserFactoryTest::CParserFactoryTest()
{

}

CParserFactoryTest::~CParserFactoryTest()
{

}

void CParserFactoryTest::testCreateParser1()
{
	CParserFactory factory;

	CPPUNIT_ASSERT( factory.createParser( "Casio61000S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Casio6100A0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "DresserWayne0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "EagleVision0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "EpsonCR0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "EpsonHRK-ZA0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "EpsonIStore0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Fujitsu1S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Fujitsu20000S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Gilbarco0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "NCR74500S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Pace0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Panda0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "RubyCR0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "RubyCR31", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Sams40S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "Sharp0S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "StarTSP7000S", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "TVS", NULL ) != NULL ); 
	CPPUNIT_ASSERT( factory.createParser( "TVS0S", NULL ) != NULL ); 
}

void CParserFactoryTest::testCreateParser2()
{
	CParserFactory factory;

	CPPUNIT_ASSERT( factory.createParser( "NotExist", NULL ) == NULL ); 
}

void CParserFactoryTest::testGetProtocol1()
{
	CParserFactory factory;

	CPPUNIT_ASSERT( factory.getFirstProtocol() != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getFirstProtocol(), "Casio61000S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Casio61000S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Casio61000S" ), "Casio6100A0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Casio6100A0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Casio6100A0S" ), "DresserWayne0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "DresserWayne0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "DresserWayne0S" ), "EagleVision0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "EagleVision0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "EagleVision0S" ), "EpsonCR0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "EpsonCR0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "EpsonCR0S" ), "EpsonHRK-ZA0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "EpsonHRK-ZA0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "EpsonHRK-ZA0S" ), "EpsonIStore0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "EpsonIStore0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "EpsonIStore0S" ), "Fujitsu1S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Fujitsu1S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Fujitsu1S" ), "Fujitsu20000S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Fujitsu20000S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Fujitsu20000S" ), "Gilbarco0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Gilbarco0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Gilbarco0S" ), "NCR74500S" ) == 0 ); 
	
	CPPUNIT_ASSERT( factory.getNextProtocol( "NCR74500S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "NCR74500S" ), "Pace0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Pace0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Pace0S" ), "Panda0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Panda0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Panda0S" ), "RubyCR0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "RubyCR0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "RubyCR0S" ), "RubyCR31" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "RubyCR31" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "RubyCR31" ), "Sams40S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Sams40S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Sams40S" ), "Sharp0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "Sharp0S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "Sharp0S" ), "StarTSP7000S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "StarTSP7000S" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "StarTSP7000S" ), "TVS" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "TVS" ) != NULL ); 
	CPPUNIT_ASSERT( strcmp( factory.getNextProtocol( "TVS" ), "TVS0S" ) == 0 ); 

	CPPUNIT_ASSERT( factory.getNextProtocol( "TVS0S" ) == NULL ); 
}
