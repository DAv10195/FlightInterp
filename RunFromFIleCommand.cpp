//RunFromFileCommand object implementation
#include "Command.h"
#include "FlightInterp.h"
#include "Utils.h"
//execute method
double RunFromFileCommand :: execute()
{
	double retCmd = 0;
	Lexer* lexer = new Lexer();
	Parser* parser = new Parser();	//accessing the file path that should be inputed next to the "run" command
	string path = this->params[this->ind + 1];

	retCmd = execFromFile(this->sTable, path, lexer, parser);

	delete lexer;
	delete parser;

	return retCmd;
}



