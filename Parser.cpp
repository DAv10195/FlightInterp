//implementation of Parser object.
#include "FlightInterp.h"
#include "Utils.h"
#include <map>
//initialize command map
void initCmdMap(map<string, Command*> &commands)
{
	Command* c = new OpenDataServerCommand();
	string key = "openDataServer";
	commands[key] = c;
}
//build conditioned command. An empty vector returned indicates failure.
vector<Command*> buildCondCmd(ShuntingYarder* sy ,map<string, Command*> &cmd, vector<string> &input,
unsigned int &beginInd, unsigned int &endInd, map<string, double> &st)
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
				c = cmd[input[i]];
				i++;
				cond = ifCond(input[i]);
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
				tmp = buildCondCmd(sy, cmd, input, i, k, st);
				c->setCommands(tmp);
				c->setCondPar(cp);
				toRet.push_back(c);
				i = k;
			}	//other command
			else
			{	//build the command.
				c = cmd[input[i]];
				c->setTable(st);
				c->setInd(i);
				c->setParams(input);
				c->setShuntingYarder(sy);
				toRet.push_back(c);
			}
		}
	}
	return toRet;
}
//constructor
Parser :: Parser()
{
	this->Shunter = new ShuntingYarder();
}
//sets the vector to be Parsed (that returned from the Lexer object)
void Parser :: setVecToParse(vector<string> &toParse)
{
	this->input = toParse;
}
//Parses the vector of the lexed input
void Parser :: Parse()
{
	map<string, Command*> commands;
	map<string, double> vars;
	Command* c = nullptr;
	Expression* e = nullptr;
	vector<string> inp = this->input;
	vector<string> cond;
	vector<Command*> tmp;
	double commandRet = 0;
	unsigned int i = 0, j = 0, counter = 0;
	unsigned int size = inp.size();
	bool flag = false;

	initCmdMap(commands);

	for (; i < size; i++)
	{	//building a vector of command expressions to be executed
		if (commands.find(inp[i]) != commands.end())
		{	//case while or if command
			if (inp[i] == "while" || inp[i] == "if")
			{
				c = commands[inp[i]];
				i++;
				cond = ifCond(inp[i]);
				if (!cond.size())
				{	//case the condition is invalid...
					cout << "Invalid condition given to " << inp[i - 1] << " command" << endl;
					return;
				}
				//building the boolean Expression and Condition parser
				e = new BooleanExpression(vars, cond, this->Shunter);
				ConditionParser* cp = new ConditionParser(e);
				//case no '{' to open the block
				i++;
				if (inp[i] != "{")
				{
					cout << "missing '{'" << endl;
					return;
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
					return;
				}
				//building the vector of commands inside the block
				tmp = buildCondCmd(this->Shunter, commands, inp, i, j, vars);
				c->setCommands(tmp);
				c->setCondPar(cp);
				i = j;
			}
			else
			{	//build command
				c = commands[inp[i]];
				c->setTable(vars);
				c->setInd(i);
				c->setParams(inp);
				c->setShuntingYarder(this->Shunter);
			}
		}
		if (c != nullptr)
		{
			e = new CommandExpression(c);
			commandRet = e->calculate();
			if (commandRet)
			{	//case command failed, we'll stop
				delete e;
				return;
			}
			delete e;
		}
	}
}
//destructor
Parser :: ~Parser()
{
	delete this->Shunter;
}
