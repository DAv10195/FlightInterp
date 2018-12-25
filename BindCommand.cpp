//Implementation of AssignCommand object
#include "Command.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double BindCommand :: execute()
{
	pthread_mutex_t* lock = (this->tAl)->lock;
	string path = "";
	if (this->ind == this->params.size() - 1 || this->ind == 0)
	{
		cout << "insufficient arguments for bind function" << endl;
		return FAIL;
	}//case not preeceded by '='
	if (this->params[this->ind - 1] != "=")
	{
		cout << "missing = before bind" << endl;
		return FAIL;
	}
	path = this->params[this->ind + 1];
	if (path.at(0) == '\"' && path.at(path.size() - 1) == '\"')
	{	//get rid of " "
		path = path.substr(1, path.size() - 2);
	}

	pthread_mutex_lock(lock);

	//path is at index + 1, variable is at index - 2 ('=' in the middle)
	(*this->refs)[path] = this->params[this->ind - 2];

	pthread_mutex_unlock(lock);

	//bind in the reverse map
	(*this->revRefs)[this->params[this->ind - 2]] = path;

	return SUCCESS;
}
