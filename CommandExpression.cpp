//CommandExpression class implementation
#include "Command.h"
//constructor
CommandExpression :: CommandExpression(Command* c)
{
	this->cmd = c;
}
//calculate method which calls the inner commands execute method
double CommandExpression :: calculate()
{
	return (this->cmd)->execute();
}
//destructor
CommandExpression :: ~CommandExpression()
{
	delete this->cmd;
}
