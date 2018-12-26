//Implementation of SleepCommand object
#include "Command.h"
#include "Utils.h"
#include <thread>
#include <chrono>
#define SUCCESS 0
#define FAIL 1
//execute method
double SleepCommand :: execute()
{
	if (this->ind == this->params.size() - 1)
	{
		cout << "no argument passed to sleep command" << endl;
		return FAIL;
	}
	string timeToSleep = this->params[this->ind + 1];
	(this->sy)->SetExpToShunt(timeToSleep);
	Expression* e = (this->sy)->Shunt();
	if (e == nullptr)
	{
		cout << "invalid argument passed to sleep command" << endl;
		return FAIL;
	}
	double val = 0;
	try
	{
		val = e->calculate();
	}
	catch (runtime_error &error)
	{
		delete e;
		cout << error.what() << endl;
		return FAIL;
	}
	delete e; //sleep for microseconds
	usleep((long)val * 1000);

	return SUCCESS;
}


