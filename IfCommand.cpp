//ifCommand object implementation
#include "Command.h"
#include "FlightInterp.h"
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
			if (cmdRet)
			{	//case one of the inner commands failed
				return 1;
			}
		}
	}
	return 0;
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