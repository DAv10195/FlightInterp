//implementation of Parser object.
#include "FlightInterp.h"
#include "Utils.h"
#include <map>
#define FAIL 1
#define SUCCESS 0
#define EXIT 2
//the following are functions returning new command objects
Command* createOpenDataServerCommand()
{
	return new OpenDataServerCommand();
}

Command* createWhileCommand()
{
	return new WhileCommand();
}

Command* createIfCommand()
{
	return new IfCommand();
}

Command* createExitCommand()
{
	return new ExitCommand();
}

Command* createRunFromFileCommand()
{
	return new RunFromFileCommand();
}

Command* createPrintCommand()
{
	return new PrintCommand();
}

Command* createConnectCommand()
{
	return new ConnectCommand();
}

Command* createVarCommand()
{
	return new VarCommand();
}

Command* createAssignCommand()
{
	return new AssignCommand();
}

Command* createBindCommand()
{
	return new BindCommand();
}

Command* createSleepCommand()
{
	return new SleepCommand();
}
//initialize command map
void initCmdMap(map<string, Command*(*)(void)> &commands)
{
	commands["openDataServer"] = &createOpenDataServerCommand;
	commands["while"] = &createWhileCommand;
	commands["if"] = &createIfCommand;
	commands["exit"] = &createExitCommand;
	commands["run"] = &createRunFromFileCommand;
	commands["print"] = &createPrintCommand;
	commands["connect"] = &createConnectCommand;
	commands["var"] = &createVarCommand;
	commands["="] = &createAssignCommand;
	commands["bind"] = &createBindCommand;
	commands["sleep"] = &createSleepCommand;
}
//build conditioned command. An empty vector returned indicates failure.
vector<Command*> buildCondCmd(ShuntingYarder* sy ,map<string, Command*(*)(void)> &cmd, vector<string> &input,
unsigned int &beginInd, unsigned int &endInd, map<string, double>* st, bool* ir, threadsAndLock* t, int* si)
{
	vector<Command*> toRet;
	vector<Command*> tmp;
	unsigned int i = beginInd, j = endInd, k = 0;
	Command* c = nullptr;
	Expression* e = nullptr;
	vector<string> cond;

	for (; i < j; i++)
	{
		if (cmd.find(input[i]) != cmd.end())
		{
			if (input[i] == "while" || input[i] == "if")
			{
				c = cmd[input[i]]();
				i++;
				cond = ifCond(st, input[i]);
				if (!cond.size())
				{	//case the condition is invalid...
					cout << "Invalid condition given to " << input[i - 1] << " command" << endl;
					toRet.clear();
					return toRet;
				}
				//building the boolean Expression and Condition parser
				e = new BooleanExpression(st, cond, sy);
				ConditionParser* cp = new ConditionParser(e);
				//case '{' is'nt in place
				i++;
				if (input[i] != "{")
				{
					cout << "'{' not in the right place" << endl;
					toRet.clear();
					return toRet;
				}
				//finding the '}'
				i++;
				k = i;
				while (input[k] != "}")
				{
					k++;
				}
				tmp = buildCondCmd(sy, cmd, input, i, k, st, ir, t, si);
				c->setCommands(tmp);
				c->setCondPar(cp);
				toRet.push_back(c);
				i = k;
			}	//other command
			else
			{	//build the command.
				c = cmd[input[i]]();
				c->setTable(st);
				c->setInd(i);
				c->setIfRun(ir);
				c->setThreadsAndLock(t);
				c->setSockId(si);
				c->setParams(input);
				c->setShuntingYarder(sy);
				toRet.push_back(c);
			}
		}
	}
	return toRet;
}
//constructor
Parser :: Parser(bool* ir, threadsAndLock* t, int* si)
{
	this->ifRun = ir;
	this->tAl = t;
	this->socketId = si;
	this->Shunter = new ShuntingYarder();
}
//sets the vector to be Parsed (that returned from the Lexer object)
void Parser :: setVecToParse(vector<string> &toParse)
{
	this->input = toParse;
}
//Parses the vector of the lexed input
double Parser :: Parse(map<string, double>* sTable)
{
	map<string, Command*(*)(void)> commands;
	Command* c = nullptr;
	Expression* e = nullptr;
	vector<string> inp = this->input;
	vector<string> cond;
	vector<Command*> tmp;
	double commandRet = 0;
	unsigned int i = 0, j = 0, counter = 0, cmdCount = 0;
	unsigned int size = inp.size();
	bool flag = false;

	initCmdMap(commands);

	for (; i < size; i++)
	{	//building a vector of command expressions to be executed
		if (commands.find(inp[i]) != commands.end())
		{	//case while or if command
			if (inp[i] == "while" || inp[i] == "if")
			{
				c = commands[inp[i]]();
				i++;
				cond = ifCond(sTable, inp[i]);
				if (!cond.size())
				{	//case the condition is invalid...
					cout << "Invalid condition given to " << inp[i - 1] << " command" << endl;
					return FAIL;
				}
				//building the boolean Expression and Condition parser
				e = new BooleanExpression(sTable, cond, this->Shunter);
				ConditionParser* cp = new ConditionParser(e);
				//case no '{' to open the block
				i++;
				if (inp[i] != "{")
				{
					cout << "missing '{'" << endl;
					return FAIL;
				}
				//looking for '}' to close the block
				i++;
				j = i;
				for (; j < size; j++)
				{	//case we saw another conditioned command with a block, we'll count openers and closers
					if (inp[i] == "{")
					{
						counter++;
					}
					if (inp[j] == "}" && counter)
					{
						counter--;
					}	//case a valid number of openers and closers is present
					if (inp[j] == "}" && !counter)
					{
						flag = true;
						break;
					}
				}
				counter = 0;
				//case no '}' to close a block
				if (!flag)
				{
					cout << "missing '}'" << endl;
					return FAIL;
				}
				//building the vector of commands inside the block
				tmp = buildCondCmd(this->Shunter, commands, inp, i, j, sTable, this->ifRun, this->tAl, this->socketId);
				c->setCommands(tmp);
				c->setCondPar(cp);
				i = j;
			}
			else
			{	//build command
				c = commands[inp[i]]();
				c->setTable(sTable);
				c->setInd(i);
				c->setParams(inp);
				c->setIfRun(this->ifRun);
				c->setThreadsAndLock(this->tAl);
				c->setSockId(this->socketId);
				c->setShuntingYarder(this->Shunter);
			}
		}
		if (c != nullptr)
		{
			cmdCount++;
			e = new CommandExpression(c);
			commandRet = e->calculate();
			if (commandRet == FAIL)
			{	//case command failed, we'll stop
				delete e;
				return FAIL;
			}
			if (commandRet == EXIT)
			{
				delete e;
				return EXIT;
			}
			delete e;
			c = nullptr;
		}
	}
	if (!cmdCount)
	{
		cout << "no command found to execute" << endl;
	}
	(this->input).clear();
	return SUCCESS;
}
//destructor
Parser :: ~Parser()
{
	delete this->Shunter;
}
