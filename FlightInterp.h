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
//parameters that will be passed to threads
struct threadParams
{
	map<string, double>* sTable;
	map<string, string>* refs;
	bool* ifRun;
	pthread_mutex_t* lock;
	int sockfd;
	int hz;
};
typedef struct threadParams threadParams;
//parameters that will be passed to threads
struct threadsAndLock
{
	pthread_t* thread;
	pthread_mutex_t* lock;
};
typedef struct threadsAndLock threadsAndLock;

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
	bool* ifRun;
	threadsAndLock* tAl;
	int* socketId;

	virtual vector<Command*> buildCondCmd(unsigned int i, unsigned int j, map<string, double>* sTable,
			map<string, string>* refs, map<string, string>* revRefs, map<string, Command*(*)(void)> &commands);

	public:
		Parser(bool* ir, threadsAndLock* t, int* si);
		virtual void setVecToParse(vector<string> &toParse);
		virtual double Parse(map<string, double>* sTable, map<string, string>* refs, map<string, string>* revRefs);
		virtual ~Parser();
};

#endif
