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
	bool* connection;
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
	map<string, Command*(*)(void)> commands;
	vector<string> input;
	bool* ifRun;
	bool* ifCreated;
	threadsAndLock* tAl;
	int* socketId;

	void initCmdMap(map<string, Command*(*)(void)> &commands);
	virtual vector<Command*> buildCondCmd(unsigned int i, unsigned int j, map<string, double>* sTable,
			map<string, string>* refs, map<string, string>* revRefs);

	public:
		Parser(bool* ic ,bool* ir, threadsAndLock* t, int* si);
		virtual void setVecToParse(vector<string> &toParse);
		virtual double Parse(map<string, double>* sTable, map<string, string>* refs, map<string, string>* revRefs);
		virtual ~Parser();
};

#endif
