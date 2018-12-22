//ifCommand object implementation
#include "Command.h"
#include "FlightInterp.h"
#define FAIL 1
#define SUCCESS 0
#define EXIT 2
//execute method
double IfCommand :: execute()
{
	ConditionParser* cp = this->cp;
	double cmdRet = 0;
	vector<Command*> commands = this->commands;
	unsigned int size = commands.size(), i = 0;
	if (cp->Parse())
	{
		for (; i < size; i++)
		{
			cmdRet = commands[i]->execute();
			if (cmdRet == FAIL)
			{	//case one of the inner commands failed
				return FAIL;
			}
			if (cmdRet == EXIT)
			{	//case exit command encountered
				return EXIT;
			}
		}
	}
	return SUCCESS;
}
//destructor
IfCommand :: ~IfCommand()
{
	unsigned int i = 0, size = (this->commands).size();
	for (; i < size; i++)
	{
		delete this->commands[i];
	}
	if (this->cp != nullptr)
	{
		delete this->cp;
	}
}
