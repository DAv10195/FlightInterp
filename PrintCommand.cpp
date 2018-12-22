//PrintCommand object implementation
#include "Command.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double PrintCommand :: execute()
{
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
		if ((this->sTable)->find(param) == (this->sTable)->end())
		{
			cout << "invalid argument passed to print function" << endl;
			return FAIL;
		}

		cout << (*this->sTable)[param] << endl;
	}



	return SUCCESS;
}



