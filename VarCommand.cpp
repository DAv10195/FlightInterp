//Implementation of AssignCommand object
#include "Command.h"
#include "Utils.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double VarCommand :: execute()
{
	if (this->ind == this->params.size() - 1)
	{
		cout << "missing variable to declare";
	}
	string varName = this->params[this->ind + 1];
	if (isDig(varName.at(0)))
	{
		cout << "illegal variable name" << endl;
	}
	//case variable is already declared...
	pthread_mutex_t* lock = (this->tAl)->lock;

	pthread_mutex_lock(lock);

	if ((this->sTable)->find(varName) != (this->sTable)->end())
	{
		pthread_mutex_unlock(lock);
		cout << "var " << varName << " already declared" << endl;
		return FAIL;
	}

	pthread_mutex_unlock(lock);
	//case no var name to declare...
	if (this->ind == (this->params).size() - 1)
	{
		cout << "no variable passed for declaration" << endl;
		return FAIL;
	}
	//new var initialized to zero
	pthread_mutex_lock(lock);

	(*this->sTable)[varName] = 0;

	pthread_mutex_unlock(lock);

	return SUCCESS;
}
