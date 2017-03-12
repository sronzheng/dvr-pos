// ParserFactory.h: interface for the CParserFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSERFACTORY_H__1D7EEDA2_D887_4968_8582_C8809362FD5A__INCLUDED_)
#define AFX_PARSERFACTORY_H__1D7EEDA2_D887_4968_8582_C8809362FD5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "parser.h"

typedef CParser *( *PARSER_CREATOR )( CReceiver *);

typedef struct _PARSER_NAME_CREATOR_PAIR
{
	char name[32];
	PARSER_CREATOR creator;
} PARSER_NAME_CREATOR_PAIR;

/*
* Parser factory. Create new specified parser. 		
*/
class CParserFactory  
{
public:

	/*
	* Function:		
	*				create new parser of specified type.
	* Parameters:
	*	parseName:	name of the parser type.
	*	receiver:	pointer to receiver, used to create parser.
	* Return value:
	*				pointer to new parser. 
	*/
	CParser *createParser( char parserName[] , CReceiver *receiver );

	/*
	* Function:		
	*				get name of first protocol.
	* Return value:
	*				protocol name. return NULL if no protocol is supported.
	*/
	char *getFirstProtocol();
	
	/*
	* Function:		
	*				get name of next protocol.
	* Parameters:
	*	pCurrProtocol:	protocol name
	* Return value:
	*				protocol name. 
	*				return NULL if the end of protocol list is reached.
	*/
	char *getNextProtocol( char *curProtocol );	

	CParserFactory();

	~CParserFactory();

private:

	static PARSER_NAME_CREATOR_PAIR parsers[];
};

#endif // !defined(AFX_PARSERFACTORY_H__1D7EEDA2_D887_4968_8582_C8809362FD5A__INCLUDED_)
