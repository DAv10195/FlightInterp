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
void Parser:: initCmdMap(map<string, Command*(*)(void)> &commands)
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
vector<Command*> Parser :: buildCondCmd(unsigned int i, unsigned int j, map<string, double>* sTable,
		map<string, string>* refs, map<string, string>* revRefs)
{
	vector<Command*> toRet;
	vector<Command*> tmp;
	unsigned int k = 0;
	Command* c = nullptr;
	Expression* e = nullptr;
	vector<string> cond;

	for (; i < j; i++)
	{
		if ((this->commands).find(this->input[i]) != (this->commands).end())
		{
			if (this->input[i] == "while" || this->input[i] == "if")
			{
				c = this->commands[input[i]]();
				i++;
				cond = ifCond((this->tAl)->lock, sTable, this->input[i]);
				if (!cond.size())
				{	//case the condition is invalid...
					cout << "Invalid condition given to " << this->input[i - 1] << " command" << endl;
					toRet.clear();
					return toRet;
				}
				//building the boolean Expression and Condition parser
				e = new BooleanExpression((this->tAl)->lock ,sTable, cond, this->Shunter);
				ConditionParser* cp = new ConditionParser(e);
				//case '{' is'nt in place
				i++;
				if (this->input[i] != "{")
				{
					cout << "'{' not in the right place" << endl;
					toRet.clear();
					return toRet;
				}
				//finding the '}'
				i++;
				k = i;
				while (this->input[k] != "}")
				{
					k++;
				}
				tmp = this->buildCondCmd(i, j, sTable, refs, revRefs);
				c->setCommands(tmp);
				c->setCondPar(cp);
				toRet.push_back(c);
				i = k;
			}	//other command
			else
			{	//build the command.
				c = commands[this->input[i]]();
				c->setTable(sTable);
				c->setRefs(refs);
				c->setRevRefs(revRefs);
				c->setInd(i);
				c->setIfRun(this->ifRun);
				c->setIfCreated(this->ifCreated);
				c->setThreadsAndLock(this->tAl);
				c->setSockId(this->socketId);
				c->setParams(this->input);
				c->setShuntingYarder(this->Shunter);
				toRet.push_back(c);
			}
		}
	}
	return toRet;
}
//constructor
Parser :: Parser(bool* ic ,bool* ir, threadsAndLock* t, int* si)
{
	this->ifCreated = ic;
	this->ifRun = ir;
	this->tAl = t;
	this->socketId = si;
	this->Shunter = new ShuntingYarder();
	this->initCmdMap(this->commands);
}
//sets the vector to be Parsed (that returned from the Lexer object)
void Parser :: setVecToParse(vector<string> &toParse)
{
	this->input = toParse;
}
//Parses the vector of the lexed input
double Parser :: Parse(map<string, double>* sTable, map<string, string>* refs, map<string, string>* revRefs)
{
	Command* c = nullptr;
	Expression* e = nullptr;
	vector<string> cond;
	vector<Command*> tmp;
	double commandRet = 0;
	unsigned int i = 0, j = 0, counter = 0, cmdCount = 0;
	unsigned int size = this->input.size();
	bool flag = false;

	initCmdMap(commands);

	for (; i < size; i++)
	{	//building a vector of command expressions to be executed
		if ((this->commands).find(this->input[i]) != (this->commands).end())
		{	//case while or if command
			if (this->input[i] == "while" || this->input[i] == "if")
			{
				c = this->commands[this->input[i]]();
				i++;
				cond = ifCond((this->tAl)->lock, sTable, this->input[i]);
				if (!cond.size())
				{	//case the condition is invalid...
					cout << "Invalid condition given to " << this->input[i - 1] << " command" << endl;
					return FAIL;
				}
				//building the boolean Expression and Condition parser
				e = new BooleanExpression((this->tAl)->lock ,sTable, cond, this->Shunter);
				ConditionParser* cp = new ConditionParser(e);
				//case no '{' to open the block
				i++;
				if (this->input[i] != "{")
				{
					cout << "missing '{'" << endl;
					return FAIL;
				}
				//looking for '}' to close the block
				i++;
				j = i;
				for (; j < size; j++)
				{	//case we saw another conditioned command with a block, we'll count openers and closers
					if (this->input[i] == "{")
					{
						counter++;
					}
					if (this->input[j] == "}" && counter)
					{
						counter--;
					}	//case a valid number of openers and closers is present
					if (this->input[j] == "}" && !counter)
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
				tmp = this->buildCondCmd(i, j, sTable, refs, revRefs);
				if (!tmp.size())
				{
					return FAIL;
				}
				c->setCommands(tmp);
				c->setCondPar(cp);
				i = j;
			}
			else
			{	//build command
				c = commands[this->input[i]]();
				c->setTable(sTable);
				c->setRefs(refs);
				c->setRevRefs(revRefs);
				c->setInd(i);
				c->setParams(this->input);
				c->setIfRun(this->ifRun);
				c->setIfCreated(this->ifCreated);
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
