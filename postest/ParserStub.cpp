// ParserStub.cpp: implementation of the CParserStub class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParserStub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParserStub::CParserStub()
 : CParser( NULL )
{

}

CParserStub::~CParserStub()
{

}

void CParserStub::parseLine( char buf[] )
{
	m_lines.push_back( buf );
}