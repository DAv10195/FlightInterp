//BooleanExpression object implementation
#include "Expression.h"
#include "Utils.h"
#define FIRST_OP 0
#define SECOND_OP 2
#define OP 1
//constructor
BooleanExpression :: BooleanExpression(pthread_mutex_t* l, map<string, double>* m, vector<string> &v, ShuntingYarder* sy)
{
	this->lock = l;
	this->sTable = m;
	this->toCalc = v;
	this->Shunter = sy;
}

double BooleanExpression :: calculate()
{
	pthread_mutex_t* lock = this->lock;
	double operand1 = 0, operand2 = 0;
	string var1 = this->toCalc[FIRST_OP];
	string var2 = this->toCalc[SECOND_OP];
	string op = this->toCalc[OP];

	if (isNum(var1))
	{
		operand1 = stod(var1);
	}
	else
	{
		pthread_mutex_lock(lock);

		operand1 = (this->sTable)->at(var1);

		pthread_mutex_unlock(lock);
	}

	if (isNum(var2))
	{
		operand2 = stod(var2);
	}
	else
	{
		pthread_mutex_lock(lock);

		operand2 = (this->sTable)->at(var2);

		pthread_mutex_unlock(lock);
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


