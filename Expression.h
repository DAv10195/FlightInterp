//header file for Expression interface
#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "FlightInterp.h"
#include "Command.h"
#include <stdexcept>
#include <map>

class Command;
class ShuntingYarder;
class Expression;
class UnaryExpression;
class BinaryExpression;
class Num;
class Plus;
class Minus;
class Mult;
class Div;
class Neg;
class CommandExp;
class BooleanExpression;
//Expression interface
class Expression
{
	public:
		virtual double calculate() = 0;
		virtual ~Expression(){};
};
//BinaryExpression abstract class
class BinaryExpression : public Expression
{
	protected:
		Expression* right;
		Expression* left;

	public:
		BinaryExpression(Expression* l, Expression* r) { this->left = l; this->right = r; }
		~BinaryExpression() { delete this->right; delete this->left; }
};
//UnaryExpression abstract class
class UnaryExpression : public Expression
{
	protected:
		Expression* exp;

	public:
		UnaryExpression(Expression* e) { this->exp = e; }
		~UnaryExpression() { delete this->exp; }
};
//Num class
class Num : public Expression
{
	double val;

	public:
		Num(double v) { this->val = v; }
		double calculate() { return this->val; }
};
//Plus class
class Plus : public BinaryExpression
{
	public:
		Plus(Expression* l, Expression* r) : BinaryExpression(l, r){}
		double calculate() { return (this->left)->calculate() + (this->right)->calculate(); }
};
//Minus class
class Minus : public BinaryExpression
{
	public:
		Minus(Expression* l, Expression* r) : BinaryExpression(l, r){}
		double calculate() { return (this->left)->calculate() - (this->right)->calculate(); }
};
//Mult class
class Mult : public BinaryExpression
{
	public:
		Mult(Expression* l, Expression* r) : BinaryExpression(l, r){}
		double calculate() { return (this->left)->calculate() * (this->right)->calculate(); }
};
//Div class
class Div : public BinaryExpression
{
	public:
		Div(Expression* l, Expression* r) : BinaryExpression(l, r){}
		double calculate()
		{	//division by zero...
			if (!(this->right)->calculate())
			{
				throw std::runtime_error("Error: Division by zero!");
			}

			return (this->left)->calculate() / (this->right)->calculate();
		}
};
//Neg class
class Neg : public UnaryExpression
{
	public:
		Neg(Expression* e) : UnaryExpression(e){}
		double calculate() { return -1 * (this->exp)->calculate(); }
};
//Command Expression class
class CommandExpression : public Expression
{
	Command* cmd;

	public:
		CommandExpression(Command* c);
		double calculate();
		~CommandExpression();
};

class BooleanExpression : public Expression
{
	map<string, double> sTable;
	vector<string> toCalc;
	ShuntingYarder* Shunter;

	public:
		BooleanExpression(map<string, double> &m, vector<string> &v, ShuntingYarder* sy);
		double calculate();
		~BooleanExpression(){};
};

#endif
