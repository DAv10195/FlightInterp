//implementation of FlightInterp class
#include "FlightInterp.h"
#include "Utils.h"
#include <stdio.h>
#define FAIL 1
#define EXIT 2

void run(int argc, char* argv[])
{
	map<string, double> sTable;
	Lexer* lexer = new Lexer();
	Parser* parser = new Parser();
	int i = 1;
	double cmdRet = 0;
	string path = "";
	string line = "";
	vector<string> fromLexer;

	for (; i < argc; i++)
	{
		path = argv[i];
		cmdRet = execFromFile(&sTable, path, lexer, parser);
		if (cmdRet == FAIL || cmdRet == EXIT)
		{
			break;
		}
	}

	if (cmdRet == FAIL)
	{
		cout << "failure in path " << path << endl;
	}

	if (cmdRet == EXIT)
	{
		delete lexer;
		delete parser;
		return;
	}

	while (cmdRet != EXIT)
	{
		getline(cin, line);
		lexer->setStrToLex(line);
		fromLexer = lexer->lex();
		parser->setVecToParse(fromLexer);
		cmdRet = parser->Parse(&sTable);
	}

	delete lexer;
	delete parser;
}


int main(int argc, char* argv[])
{
	run(argc, argv);

	return 0;
}
