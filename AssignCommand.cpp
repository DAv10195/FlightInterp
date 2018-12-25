//Implementation of AssignCommand object
#include "Command.h"
#include "Utils.h"
#include "string.h"
#define SUCCESS 0
#define FAIL 1
#define OUT_SOCK 2
#define BUFFER_SIZE 1024
//execute method
double AssignCommand :: execute()
{
	pthread_mutex_t* lock = (this->tAl)->lock;
	//case no target
	if (this->ind == 0)
	{
		cout << "missing assignment target" << endl;
		return FAIL;
	}
	//case no source
	if (this->ind == (this->params).size() - 1)
	{
		cout << "missing assignment source" << endl;
		return FAIL;
	}

	string trg = this->params[this->ind - 1], src = this->params[this->ind + 1];
	//case we are in the middle of a declaration/binding
	if (src == "bind")
	{
		return SUCCESS;
	}
	//replace variables with their values
	src = assignVars(lock ,this->sTable, src);
	//case invalid source
	if (src == "")
	{
		cout << "invalid assignment argument" << endl;
		return FAIL;
	}
	//case target isn't declared
	pthread_mutex_lock(lock);

	if ((this->sTable)->find(trg) == (this->sTable)->end())
	{
		pthread_mutex_unlock(lock);
		cout << "assignment to undeclared variable" << endl;
		return FAIL;
	}

	pthread_mutex_unlock(lock);

	Expression* exp = nullptr;
	double val = 0;
	try
	{
		(this->sy)->SetExpToShunt(src);
		exp = (this->sy)->Shunt();
		if (exp == nullptr)
		{
			cout << "invalid assignment argument" << endl;
			return FAIL;
		}
		val = exp->calculate();
		delete exp;

	}
	catch (std::runtime_error &e)
	{
		delete exp;
		cout << e.what() << endl;
		return FAIL;
	}

	char buffer[BUFFER_SIZE];
	string path = "";

	pthread_mutex_lock(lock);

	while ((this->revRefs)->find(trg) != (this->revRefs)->end())
	{	//looking for a variable binded to other variables
		path = (*this->revRefs)[trg];
		(*this->sTable)[trg] = val;
		trg = path;
	}
	//case unbinded variable assignment
	if (path == "")
	{
		return SUCCESS;
	}
	pthread_mutex_unlock(lock);
	//send message to FlightGear
	bzero(buffer, BUFFER_SIZE);
	string message = "set " + path + " " + to_string(val) + " \r\n";
	strcpy(buffer, message.c_str());
	int status = write(this->socketId[OUT_SOCK], buffer, strlen(buffer));
	if (status < 0)
	{
		cout << "Error connecting to FlightGear" << endl;
		return FAIL;
	}

	return SUCCESS;
}
