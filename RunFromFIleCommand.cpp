//RunFromFileCommand object implementation
#include "Command.h"
#include "FlightInterp.h"
#include "Utils.h"
//execute method
double RunFromFileCommand :: execute()
{
	double retCmd = 0;
	Lexer* lexer = new Lexer();
	Parser* parser = new Parser(this->ifRun, this->tAl, this->socketId);	//accessing the file path that should be inputed next to the "run" command
	string path = this->params[this->ind + 1];

	retCmd = execFromFile(this->sTable, this->refs, this->revRefs, path, lexer, parser);

	delete lexer;
	delete parser;

	return retCmd;
}



