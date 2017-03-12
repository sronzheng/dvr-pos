// ParserStub.h: interface for the CParserStub class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSERSTUB_H__43D52C5F_310B_43A0_95CB_011FCDE07F5C__INCLUDED_)
#define AFX_PARSERSTUB_H__43D52C5F_310B_43A0_95CB_011FCDE07F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include <deque>
#include <string>

#include "parser.h"

class CParserStub : public CParser  
{
public:

	virtual void parseLine( char buf[] );

	CParserStub();
	virtual ~CParserStub();

	std::deque< std::string > m_lines;
};

#endif // !defined(AFX_PARSERSTUB_H__43D52C5F_310B_43A0_95CB_011FCDE07F5C__INCLUDED_)
