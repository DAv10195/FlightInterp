//header file for Command interface
#ifndef COMMAND_H_
#define COMMAND_H_

#include "Expression.h"
#include <vector>

using namespace std;
class Expression;
class Command;
class AssignCommand;
class OpenDataServerCommand;
class CreateVarCommand;
class BindCommand;
//Command interface
class Command
{
	vector<Expression*> params;
	int* indPtr;

	public:
		Command() { this->indPtr = nullptr; }
		virtual double execute() { return 0; };
		virtual void setParams(vector<Expression*> p) { this->params = p; };
		virtual void setIndPtr(int* i) { this->indPtr = i; };
		virtual ~Command(){};
};
//AssignCommand class
class AssignCommand : public Command
{
	public:
		AssignCommand() : Command(){}
		virtual double execute();
};
//OpenDataServerCommand class
class OpenDataServerCommand : public Command
{
	public:
		OpenDataServerCommand() : Command(){}
		virtual double execute();
};
//CreateVarCommand class
class CreateVarCommand : public Command
{
	public:
		CreateVarCommand() : Command(){}
		virtual double execute();
};
//BindCommand
class BindCommand : public Command
{
	public:
		BindCommand() : Command(){}
		virtual double execute();
};

#endif
