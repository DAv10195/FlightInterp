//implementation of Lexer object.
#include "FlightInterp.h"
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
//constructor.
Lexer :: Lexer()
{
	this->toLex = "";
}
//sets the input to be lexed.
void Lexer :: setStrToLex(string &toBeLexed)
{
	this->toLex = toBeLexed;
}
//returns a vector with the input after lexing.
vector<string> Lexer :: lex()
{
	vector<string> toRet;
	vector<string> tmp;
	string curExp = "";
	string input = this->toLex;
	bool flag = false;
	int len = input.size();
	int i = 0, j = 0;
	//split all inputed text by whitespaces.
	while (i < len)
	{	//'\n' not included because it ends an input line.
		while (i < len && (input.at(i) == ' ' || input.at(i) == '\t' || input.at(i) == '\r'))
		{
			i++;
		}
		//reading numbers
		while (i < len && isDig(input.at(i)))
		{
			flag = true;
			curExp.push_back(input.at(i));
			i++;
		}
		//if number was read we'll continue to the next iteration
		if (flag)
		{
			tmp.push_back(curExp);
			curExp = "";
			flag = false;
			continue;
		}
		//reading operators
		while (i < len && isOper(input.at(i)))
		{
			flag = true;
			curExp.push_back(input.at(i));
			i++;
		}
		//if operator was read we'll continue to the next iteration
		if (flag)
		{
			tmp.push_back(curExp);
			curExp = "";
			flag = false;
			continue;
		}
		//reading other expressions
		while (i < len && input.at(i) != ' ' && input.at(i) != '\t' && input.at(i) != '\r' && !isOper(input.at(i)))
		{
			flag = true;
			curExp.push_back(input.at(i));
			i++;
			//i.p. adresses and double values.
			if (curExp == ".")
			{
				break;
			}
		}
		//if other expression was read we'll continue to the next iteration
		if (flag)
		{
			tmp.push_back(curExp);
			curExp = "";
			flag = false;
		}
	}
	//now, we'll combine complex mathematical/logical expressions
	i = 0;
	len = tmp.size();
	while (i < len)
	{
		curExp = tmp[i];
		j = i + 1;
		//special case for assignment operator, bind or block indicators.
		if (curExp == "=" || curExp == "bind" || curExp == "{" || curExp == "}")
		{
			toRet.push_back(curExp);
			i = j;
			continue;
		}
		//if exp is number, operator, dot or followed by operator
		if (isNum(curExp) || isOper(curExp) || (j < len && isOper(tmp[j])))
		{
			if (j < len && (tmp[j] == "(" || tmp[j] == "!" || tmp[j] == "!("))
			{
				toRet.push_back(curExp);
				i = j;
				continue;
			}
			while (j < len && tmp[j] != "," && tmp[j] != "{" && tmp[j] != "}" && tmp[j] != "(")
			{	//case two numbers in a row or a number and a closing token.
				if (isNum(tmp[j]) && (isNum(tmp[j - 1]) || tmp[j - 1] == ")"))
				{
					break;
				}
				curExp += tmp[j];
				j++;
			}
		}

		toRet.push_back(curExp);
		i = j;
	}

	this->toLex = "";
	return toRet;
}
