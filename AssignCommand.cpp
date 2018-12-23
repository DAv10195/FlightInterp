//Implementation of AssignCommand object
#include "Command.h"
#include "Utils.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double AssignCommand :: execute()
{	//case no targer
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
	//case we are in the middle of a declaration
	if (src == "bind")
	{
		return SUCCESS;
	}
	//replace variables with their values
	src = assignVars(this->sTable, trg);
	//case invalid source
	if (src == "")
	{
		cout << "invalid assignment argument" << endl;
		return FAIL;
	}
	//case target isn't declared
	if ((this->sTable)->find(trg) == (this->sTable)->end())
	{
		cout << "assignment to undeclared variable" << endl;
		return FAIL;
	}

	try
	{
		(this->sy)->SetExpToShunt(src);
		(*this->sTable)[trg] = ((this->sy)->Shunt())->calculate();
	}
	catch (std::runtime_error &e)
	{
		cout << e.what() << endl;
		return FAIL;
	}

	return SUCCESS;
}



