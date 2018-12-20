//header file for Command interface
#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include <string>

using namespace std;
//Command interface
class Command
{
	public:
		virtual double execute() = 0;
		virtual ~Command(){};
};
//AssignCommand class
class AssignCommand : public Command
{
	vector<string> params;
	int* indPtr;

	public:
		AssignCommand(vector<string> &p, int* i) { this->params = p, this->indPtr = i; }
		virtual double execute();
};
//OpenDataServerCommand class
class OpenDataServerCommand : public Command
{
	vector<string> params;

	public:
		OpenDataServerCommand();
		virtual double execute();
};
//CreateVarCommand class
class CreateVarCommand : public Command
{

};
#endif
