// BinaryStreamTest.h: interface for the CBinaryStreamTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYSTREAMTEST_H__88410C70_0255_4BBE_AD39_735BD26CFD9E__INCLUDED_)
#define AFX_BINARYSTREAMTEST_H__88410C70_0255_4BBE_AD39_735BD26CFD9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/extensions/HelperMacros.h>

class CBinaryStreamTest : public CPPUNIT_NS::TestFixture   
{
	CPPUNIT_TEST_SUITE( CBinaryStreamTest );
		CPPUNIT_TEST( testReadWrite1 );
		CPPUNIT_TEST( testReadWrite2 );
		CPPUNIT_TEST( testReadWrite3 );
	CPPUNIT_TEST_SUITE_END();

public:
	void testReadWrite1();
	void testReadWrite2();
	void testReadWrite3();

	CBinaryStreamTest();
	virtual ~CBinaryStreamTest();

};

#endif // !defined(AFX_BINARYSTREAMTEST_H__88410C70_0255_4BBE_AD39_735BD26CFD9E__INCLUDED_)
