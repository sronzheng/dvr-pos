// ParserFactoryTest.h: interface for the CParserFactoryTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSERFACTORYTEST_H__CE4440E5_BAF5_42EB_B7FA_5B32279EA2A1__INCLUDED_)
#define AFX_PARSERFACTORYTEST_H__CE4440E5_BAF5_42EB_B7FA_5B32279EA2A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CParserFactoryTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( CParserFactoryTest );
		CPPUNIT_TEST( testCreateParser1 );
		CPPUNIT_TEST( testCreateParser2 );
		CPPUNIT_TEST( testGetProtocol1 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testCreateParser1();
	void testCreateParser2();
	void testGetProtocol1();

	CParserFactoryTest();
	virtual ~CParserFactoryTest();

};

#endif // !defined(AFX_PARSERFACTORYTEST_H__CE4440E5_BAF5_42EB_B7FA_5B32279EA2A1__INCLUDED_)
