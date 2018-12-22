//header file for FlightInterp class
#ifndef FLIGHTINTERP_H_
#define FLIGHTINTERP_H_

#include "Expression.h"
#include "Command.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class Expression;
class Lexer;
class ShuntingYarder;
class ConditionParser;
class Parser;
//Lexer class
class Lexer
{
	string toLex;

	public:
		Lexer();
		virtual void setStrToLex(string &toLex);
		virtual vector<string> lex();
		virtual ~Lexer(){};
};
//ShuntingYarder class
class ShuntingYarder
{
	string toShunt;

	public:
		ShuntingYarder();
		virtual void SetExpToShunt(string &s);
		virtual Expression* Shunt();
		virtual ~ShuntingYarder(){};
};
//ConditionParser class
class ConditionParser
{
	Expression* exp;

	public:
		ConditionParser(Expression* e);
		virtual double Parse();
		virtual ~ConditionParser();
};
//Parser class
class Parser
{
	ShuntingYarder* Shunter;
	vector<string> input;

	public:
		Parser();
		virtual void setVecToParse(vector<string> &toParse);
		virtual void Parse();
		virtual ~Parser();
};

#endif
