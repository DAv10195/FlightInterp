//BooleanExpression object implementation
#include "Expression.h"
#include "Utils.h"
//constructor
BooleanExpression :: BooleanExpression(map<string, double> &m, vector<string> &v, ShuntingYarder* sy)
{
	this->sTable = m;
	this->toCalc = v;
	this->Shunter = sy;
}

double BooleanExpression :: calculate()
{
	double operand1 = 0, operand2 = 0;
	string var1 = this->toCalc[0];
	string var2 = this->toCalc[2];
	string op = this->toCalc[1];

	if (isNum(var1))
	{
		operand1 = stod(var1);
	}
	else
	{
		operand1 = this->sTable.at(var1);
	}

	if (isNum(var2))
	{
		operand2 = stod(var2);
	}
	else
	{
		operand2 = this->sTable.at(var2);
	}

	if (op == "==")
	{
		return operand1 == operand2;
	}
	if (op == "!=")
	{
		return operand1 != operand2;
	}
	if (op == "<=")
	{
		return operand1 <= operand2;
	}
	if (op == ">=")
	{
		return operand1 >= operand2;
	}
	if (op == "<")
	{
		return operand1 < operand2;
	}
	if (op == ">")
	{
		return operand1 > operand2;
	}
	//we'll be reached only if invalid condition was parsed, but we'll make sure it won't happen using ifCond.
	return 0;
}


