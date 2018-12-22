//ConditionParser object implementation.
#include "FlightInterp.h"
#include "Utils.h"
//constructor
ConditionParser :: ConditionParser(Expression* e)
{
	this->exp = e;
}
//parses the Expression.
double ConditionParser :: Parse()
{
	return (this->exp)->calculate();
}

ConditionParser :: ~ConditionParser()
{
	delete this->exp;
}
