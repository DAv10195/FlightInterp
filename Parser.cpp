//implementation of Parser object.
#include "FlightInterp.h"
#include "Utils.h"
#include <map>

void initCmdMap(map<string, Command*> &commands)
{
	Command* c = new OpenDataServerCommand();
	string key = "openDataServer";
	commands[key] = c;
}
//constructor
Parser :: Parser(ShuntingYarder* sy)
{
	this->Shunter = sy;
}
//sets the vector to be Parsed (that returned from the Lexer object)
void Parser :: setVecToParse(vector<string> &toParse)
{
	this->input = toParse;
}
//Parses the vector of the lexed input
void Parser :: Parse()
{
	map<string, Command*> commands;
	map<string, double> vars;
	Command* c = nullptr;
	vector<Expression*> expressions;
	vector<string> inp = this->input;
	unsigned int i = 0;
	unsigned int size = inp.size();

}
//destructor
Parser :: ~Parser()
{
	delete this->Shunter;
}
