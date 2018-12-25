//PrintCommand object implementation
#include "Command.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double PrintCommand :: execute()
{
	pthread_mutex_t* lock = (this->tAl)->lock;
	if (this->ind == (this->params).size() - 1)
	{
		cout << "no paramater passed to print function" << endl;
		return FAIL;
	}
	string param = this->params[this->ind + 1];
	string toPrint = "";
	//case a string with "" and "" at the end was inputed.
	if ((param.at(0) == '"' || param.at(0) == '\"') && param.at(param.size() - 1) == '"')
	{
		toPrint = param.substr(1, param.size() - 2);
		cout << toPrint << endl;
		return SUCCESS;
	}
	//case a variable was inputed, or "" only at the begin
	if ((param.at(0) != '"' && param.at(param.size() - 1) != '"') || (param.at(0) == '"' || param.at(0) == '\"'))
	{	//no such variable
		pthread_mutex_lock(lock);

		if ((this->sTable)->find(param) == (this->sTable)->end())
		{
			pthread_mutex_unlock(lock);
			cout << "invalid argument passed to print function" << endl;
			return FAIL;
		}

		cout << (*this->sTable)[param] << endl;

		pthread_mutex_unlock(lock);
	}

	return SUCCESS;
}



