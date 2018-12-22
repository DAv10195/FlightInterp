//implementation of utility functions.
#include "Utils.h"
#include <fstream>
#define COND_OPERANDS 3
#define FIRST_OP 0
#define SECOND_OP 2
#define FAIL 1

//decides if the inputed character is a valid operator.
bool isOper(char &c)
{	//mathematical
	if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%' || c == '(' || c == ')')
	{
		return true;
	}
	//logical
	if (c == '>' || c == '<' || c == '!' || c == '=' || c == '&' || c == '|')
	{
		return true;
	}
	//else...
	return false;
}
//same as the function before, but for a string.
bool isOper(string &s)
{	//mathematical
	if (s == "+" || s == "-" || s == "/" || s == "*" || s == "%" || s == "(" || s == ")")
	{
		return true;
	}
	//logical
	if (s == ">" || s == ">=" || s == "<=" || s == "<" || s == "!=" || s == "==" || s == "&&" || s == "||")
	{
		return true;
	}
	//other cases
	if (s == "!" || s == "|" || s == "&")
	{
		return true;
	}
	//combinations
	if (s == "!(")
	{
		return true;
	}
	//else...
	return false;
}
//decides if the inputed character is a valid digit.
bool isDig(char &c)
{	//if not digit
	if (c < '0' || c > '9')
	{
		return false;
	}
	//else...
	return true;
}
//check if inputed string is a number.
bool isNum(string &s)
{
	int i = 0, size = s.size();
	for (; i < size; i++)
	{	//case a character is not a digit
		if (!isDig(s.at(i)))
		{
			return false;
		}
	}
	//else...
	return true;
}
//an Empty vector indicates an invalid condition. otherwise, the condition is valid.
vector<string> ifCond(map<string, double>* st, string &s)
{
	char c = 0;
	unsigned int i = 0;
	unsigned int size = s.size();
	string str = "";
	vector<string> toRet;
	string op = "";
	while (i < size)
	{	//build operands
		while (i < size && s.at(i) != '!' && s.at(i) != '<' && s.at(i) != '>' && s.at(i) != '=')
		{	//ignore tokens
			if (s.at(i) == '(' || s.at(i) == ')' || s.at(i) == ' ')
			{
				i++;
				continue;
			}
			str.push_back(s.at(i));
			i++;
		}
		if (str == "")
		{	//not enough operands...
			toRet.clear();
		}
		toRet.push_back(str);
		//case an operator was encountered
		if (i < size && (s.at(i) == '<' || s.at(i) == '>' || s.at(i) == '=' || s.at(i) == '!'))
		{
			if (op != "")
			{	//case another operator was read...
				toRet.clear();
				break;
			}
			c = s.at(i);
			if (c == '<')
			{	//case <=
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = "<=";
					op = str;
					toRet.push_back(op);
					str = "";
					i++;
				}	//case <
				else
				{
					str = "<";
					op = str;
					toRet.push_back(op);
					str = "";
				}
			}
			if (c == '>')
			{	//case >=
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = ">=";
					op = str;
					toRet.push_back(op);
					str = "";

					i++;
				}	//case >
				else
				{
					str = ">";
					op = str;
					toRet.push_back(op);
					str = "";
				}
			}
			if (c == '!')
			{	//case !=
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = "!=";
					op = str;
					toRet.push_back(op);
					str = "";
					i++;
				}	//case ! not followed by =
				else
				{
					toRet.clear();
					break;
				}
			}
			if (c == '=')
			{	//case ==
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = "==";
					op = str;
					toRet.push_back(op);
					str = "";
					i++;
				}	//case = not followed by =
				else
				{
					toRet.clear();
					break;
				}
			}
		}
		i++;
	}
	//case num of operands is not 2 or no operator was read
	if (op == "" || toRet.size() != COND_OPERANDS)
	{
		toRet.clear();
	}
	//case first operand is not a number and an undeclared variable
	if (!isNum(toRet[FIRST_OP]) && st->find(toRet[FIRST_OP]) == st->end())
	{
		cout << "var " << toRet[FIRST_OP] << " undeclared" << endl;
		toRet.clear();
	}
	//case second operand is not a number and an undeclared variable
	if (!isNum(toRet[SECOND_OP]) && st->find(toRet[SECOND_OP]) == st->end())
	{
		cout << "var " << toRet[SECOND_OP] << " undeclared" << endl;
		toRet.clear();
	}

	return toRet;
}
//assigns value to variables in the string. if a variable isn't declared returns empty string.
string assignVars(map<string, double>* st, string &s)
{
	string toRet = "";
	string var = "";
	double val = 0;
	unsigned int i = 0;
	unsigned int size = s.size();

	for (; i < size; i++)
	{	//a digit or an operator is good
		if (isOper(s.at(i)) || isDig(s.at(i)) || s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r')
		{	//ignore whitespaces
			if (s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r')
			{
				continue;
			}

			toRet.push_back(s.at(i));
		}
		else
		{	//a variable has been encountered
			while (i < size && !isOper(s.at(i)) && !isDig(s.at(i)))
			{	//build the name of the variable, ignoring whitespaces
				if (s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r')
				{
					i++;
					continue;
				}
				var.push_back(s.at(i));
				i++;
			}
			i--;
			//case variable hasn't been declared;
			if (st->find(var) == st->end())
			{
				return "";
			}
			//assign value instead of the variable to the returned string.
			val = (*st)[var];
			toRet += to_string(val);
		}
	}

	return toRet;
}
//executes the script written in the inputed file path
double execFromFile(map<string, double>* sTable, string &path, Lexer* l, Parser* p)
{
	string line = "";
	vector<string> toParse;
	vector<string> fromLexer;
	ifstream f;
	unsigned int size = 0, i = 0;

	f.open(path);
	if (!f)
	{	//invalid path...
		cout << "Invalid file path was passed" << endl;
		return FAIL;
	}
	else
	{
		while (getline(f, line))
		{	//lex all of the script in the file
			l->setStrToLex(line);
			fromLexer = l->lex();
			size = fromLexer.size();
			for (; i < size; i++)
			{
				toParse.push_back(fromLexer[i]);
			}
			i = 0;
		}
		f.close();
		p->setVecToParse(toParse);
		return p->Parse(sTable);
	}
}

