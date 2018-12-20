//header file for FlightInterp class.
#ifndef FLIGHTINTERP_H_
#define FLIGHTINTERP_H_

#include <iostream>
#include "Expression.h"

//Lexer class declaration.
class Lexer
{
	string toLex;

	public:
		Lexer();
		virtual void setStrToLex(string &toLex);
		virtual vector<string> lex();
		virtual ~Lexer(){};
};
//Parser class declaration.
class Parser
{
	vector<string> toParse;

	public:
		Parser(){};
		virtual void setVecToParse(vector<string> &toParse);
		virtual void Parse();
		virtual ~Parser(){};
};

#endif
