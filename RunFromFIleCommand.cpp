//RunFromFileCommand object implementation
#include "Command.h"
#include "FlightInterp.h"
#include "Utils.h"
#define FAIL 1
//execute method
double RunFromFileCommand :: execute()
{
	if (this->ind == (this->params).size() - 1)
	{
		cout << "no path inputed to run command" << endl;
		return FAIL;
	}
	Lexer* lexer = new Lexer();
	Parser* parser = new Parser(this->ifCreated ,this->ifRun, this->tAl, this->socketId);	//accessing the file path that should be inputed next to the "run" command
	string path = this->params[this->ind + 1];

	double retCmd = execFromFile(this->sTable, this->refs, this->revRefs, path, lexer, parser);

	delete lexer;
	delete parser;

	return retCmd;
}



