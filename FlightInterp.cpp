//implementation of FlightInterp class
#include "FlightInterp.h"
#include "Utils.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#define FAIL 1
#define EXIT 2
#define NUM_THREADS 2
#define NUM_SOCK 4

void run(int argc, char* argv[])
{
	int i = 0;
	int sockArr[NUM_SOCK];
	for (; i < NUM_SOCK; i++)
	{	//initialize sockets values
		sockArr[i] = -1;
	}
	pthread_t threads[NUM_THREADS];
	pthread_mutex_t lock;
	 if (pthread_mutex_init(&lock, NULL))
	 {
	        cout << "mutex initialization error" << endl;
	        return;
	 }
	threadsAndLock tAl;
	tAl.threads = threads;
	tAl.lock = &lock;
	bool ifRun = true;
	//pthread_create(&(threads[0]), NULL, &writeThread, (void*)&tAl);
	//pthread_create(&(threads[1]), NULL, &readThread, (void*)&tAl);
	map<string, double> sTable;
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
		cmdRet = execFromFile(&sTable, path, lexer, parser);
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
		ifRun = false;
		i = 0;
		for (; i < NUM_THREADS; i++)
		{
			pthread_join(threads[i], NULL);
		}
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
		cmdRet = parser->Parse(&sTable);
	}
	ifRun = false;
	delete lexer;
	delete parser;
}

int main(int argc, char* argv[])
{
	run(argc, argv);

	return 0;
}
