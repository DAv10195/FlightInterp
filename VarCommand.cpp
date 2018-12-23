//Implementation of AssignCommand object
#include "Command.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double VarCommand :: execute()
{
	string varName = this->params[this->ind + 1];
	//case variable is already declared...
	if ((this->sTable)->find(varName) != (this->sTable)->end())
	{
		cout << "var " << varName << " already declared" << endl;
		return FAIL;
	}
	//case no var name to declare...
	if (this->ind == (this->params).size() - 1)
	{
		cout << "no variable passed for declaration" << endl;
		return FAIL;
	}
	//new var initialized to zero
	(*this->sTable)[varName] = 0;

	return SUCCESS;
}
