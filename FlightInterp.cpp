//implementation of FlightInterp class
#include "FlightInterp.h"
#include "Utils.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#define FAIL 1
#define EXIT 2
#define NUM_SOCK 3
#define NUM_BOOLS 2
//runs the whole deal
void run(int argc, char* argv[])
{
	cout << "Welcome to FlightInterp 1.0 - FlightGear interpreter" << endl;
	cout << "(make sure you run this interpreter BEFORE running FlightGear)" << endl;
	int i = 0;
	int sockArr[NUM_SOCK];
	for (; i < NUM_SOCK; i++)
	{	//initialize sockets values
		sockArr[i] = -1;
	}
	pthread_t thread;
	pthread_mutex_t lock;
	if (pthread_mutex_init(&lock, NULL))
	{
        cout << "mutex initialization error" << endl;
        return;
	}
	threadsAndLock tAl;
	tAl.thread = &thread;
	tAl.lock = &lock;
	bool ifRun = true;
	map<string, double> sTable;
	map<string, string> refs;
	map<string, string> revRefs;
	Lexer* lexer = new Lexer();
	Parser* parser = new Parser(&ifRun, &tAl, sockArr);
	i = 1;
	double cmdRet = 0;
	string path = "";
	string line = "";
	vector<string> fromLexer;
	//executing scripts from all file paths passed as arguments
	for (; i < argc; i++)
	{
		path = argv[i];
		cmdRet = execFromFile(&sTable, &refs, &revRefs, path, lexer, parser);
		if (cmdRet == FAIL || cmdRet == EXIT)
		{
			break;
		}
	}
	//case a failure occured
	if (cmdRet == FAIL)
	{
		cout << "failure in path " << path << endl;
	}

	if (cmdRet == EXIT)
	{	//destruct all system resources
		delete lexer;
		delete parser;

		pthread_mutex_lock(&lock);

		ifRun = false;

		pthread_mutex_unlock(&lock);

		pthread_join(thread, NULL);
		pthread_mutex_destroy(&lock);
		i = 0;
		for (; i < NUM_SOCK; i++)
		{
			if (sockArr[i] >= 0)
			{
				close(sockArr[i]);
			}
		}
		return;
	}

	while (cmdRet != EXIT)
	{
		getline(cin, line);
		lexer->setStrToLex(line);
		fromLexer = lexer->lex();
		parser->setVecToParse(fromLexer);
		cmdRet = parser->Parse(&sTable, &refs, &revRefs);
	}
	delete lexer;
	delete parser;

	pthread_mutex_lock(&lock);

	ifRun = false;

	pthread_mutex_unlock(&lock);

	pthread_join(thread, NULL);
	pthread_mutex_destroy(&lock);
	i = 0;
	for (; i < NUM_SOCK; i++)
	{
		if (sockArr[i] >= 0)
		{
			close(sockArr[i]);
		}
	}
}

int main(int argc, char* argv[])
{
	run(argc, argv);

	return 0;
}
