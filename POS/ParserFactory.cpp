// ParserFactory.cpp: implementation of the CParserFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParserFactory.h"

#include "EagleVision0SParser.h"
#include "TVS0SParser.h"
#include "Sharp0SParser.h"
#include "RubyCR0SParser.h"
#include "Panda0SParser.h"
#include "Pace0SParser.h"
#include "NCR74500SParser.h"
#include "Gilbarco0SParser.h"
#include "Fujitsu1SParser.h"
#include "EpsonHRK_ZA0SParser.h"
#include "EpsonCR0SParser.h"
#include "DresserWayne0SParser.h"
#include "Casio6100A0SParser.h"
#include "Casio61000SParser.h"
#include "EpsonIStore0SParser.h"
#include "Fujitsu20000SParser.h"
#include "Sams40SParser.h"
#include "StarTSP7000SParser.h"

static CParser *create_EagleVision0S_Parser( CReceiver *receiver )
{
	return new CEagleVision0SParser( receiver );
}

static CParser *create_TVS0S_Parser( CReceiver *receiver )
{
	return new CTVS0SParser( receiver );
}

static CParser *create_SHARP0S_Parser( CReceiver *receiver )
{
	return new CSharp0SParser( receiver );
}

static CParser *create_RubyCR0S_Parser( CReceiver *receiver )
{
	return new CRubyCR0SParser( receiver );
}

static CParser *create_Panda0S_Parser( CReceiver *receiver )
{
	return new CPanda0SParser( receiver );
}

static CParser *create_Pace0S_Parser( CReceiver *receiver )
{
	return new CPace0SParser( receiver );
}

static CParser *create_NCR74500S_Parser( CReceiver *receiver )
{
	return new CNCR74500SParser( receiver );
}

static CParser *create_Gilbarco0S_Parser( CReceiver *receiver )
{
	return new CGilbarco0SParser( receiver );
}

static CParser *create_Fujitsu1S_Parser( CReceiver *receiver )
{
	return new CFujitsu1SParser( receiver );
}

static CParser *create_EpsonHRK_ZA0S_Parser( CReceiver *receiver )
{
	return new CEpsonHRK_ZA0SParser( receiver );
}

static CParser *create_EpsonCR0S_Parser( CReceiver *receiver )
{
	return new CEpsonCR0SParser( receiver );
}

static CParser *create_DresserWayne0S_Parser( CReceiver *receiver )
{
	return new CDresserWayne0SParser( receiver );
}

static CParser *create_Casio6100A0S_Parser( CReceiver *receiver )
{
	return new CCasio6100A0SParser( receiver );
}

static CParser *create_Casio61000S_Parser( CReceiver *receiver )
{
	return new CCasio61000SParser( receiver );
}

static CParser *create_EpsonIStore0S_Parser( CReceiver *receiver )
{
	return new CEpsonIStore0SParser( receiver );
}

static CParser *create_Fujitsu20000S_Parser( CReceiver *receiver )
{
	return new CFujitsu20000SParser( receiver );
}

static CParser *create_Sams40S_Parser( CReceiver *receiver )
{
	return new CSams40SParser( receiver );
}

static CParser *create_StarTSP7000S_Parser( CReceiver *receiver )
{
	return new CStarTSP7000SParser( receiver );
}

PARSER_NAME_CREATOR_PAIR CParserFactory::parsers[] = 
{
	"Casio61000S",		create_Casio61000S_Parser,
	"Casio6100A0S",		create_Casio6100A0S_Parser,
	"DresserWayne0S",	create_DresserWayne0S_Parser,
	"EagleVision0S",	create_EagleVision0S_Parser,
	"EpsonCR0S",		create_EpsonCR0S_Parser,		
	"EpsonHRK-ZA0S",	create_EpsonHRK_ZA0S_Parser,		
	"EpsonIStore0S",	create_EpsonIStore0S_Parser,		
	"Fujitsu1S",		create_Fujitsu1S_Parser,		
	"Fujitsu20000S",	create_Fujitsu20000S_Parser,		
	"Gilbarco0S",		create_Gilbarco0S_Parser,		
	"NCR74500S",		create_NCR74500S_Parser,		
	"Pace0S",			create_Pace0S_Parser,
	"Panda0S",			create_Panda0S_Parser,
	"RubyCR0S",			create_RubyCR0S_Parser,
	"RubyCR31",			create_RubyCR0S_Parser,			// same with RubyCR0S
	"Sams40S",			create_Sams40S_Parser,
	"Sharp0S",			create_SHARP0S_Parser,
	"StarTSP7000S",		create_StarTSP7000S_Parser,
	"TVS",				create_TVS0S_Parser,			// same with TVS0S
	"TVS0S",			create_TVS0S_Parser,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParserFactory::CParserFactory()
{
}

CParserFactory::~CParserFactory()
{
}

CParser *CParserFactory::createParser( char parserName[] , 
									   CReceiver *receiver )
{
	for( int i = 0; 
	     i < sizeof( parsers ) / sizeof( parsers[0] ); 
		 i++)
	{
		if( strcmp( parsers[i].name, parserName ) == 0)
			return (*parsers[i].creator)( receiver ); 
	}

	return NULL;
}

char *CParserFactory::getFirstProtocol()
{
	return parsers[0].name;
}

char *CParserFactory::getNextProtocol( char *curProtocol )
{
	for( int i = 0; 
		 i < sizeof( parsers ) / sizeof( parsers[0] ) - 1 ; 
		 i++)
	{
		if( strcmp( parsers[i].name, curProtocol ) == 0)
			return parsers[i + 1].name;
	}

	return NULL;
}