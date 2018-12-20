//Implementation of ShuntingYard algorithm.
#include <bits/stdc++.h>
#include <stack>
#include "FlightInterp.h"

using namespace std;


//choose which operator to create during run time.

Expression *chooseOp(Expression *leftVal, Expression *rightVal, char op){
    switch(op){
        case '+': return new Plus(leftVal, rightVal);
        case '-': return new Minus(leftVal, rightVal);
        case '*': return new Mult(leftVal, rightVal);
        case '/': return new Div(leftVal, rightVal);
    }
}


// Function to find precedence of operator.
	int precedence(char op){
		if(op == '+'||op == '-'){
			return 1;
		}
		if(op == '*'||op == '/'){
			return 2;
		}
    return 0;
}

	//initialize empty string.

ShuntingYarder::ShuntingYarder(){
	this->toShunt="";
}
//set the string.
void ShuntingYarder:: setExpToShunt(string &s){
	this->toShunt = s;
}
//the algorithm.
Expression* ShuntingYarder::Shunt(){
	int i;
	bool isNeg = false;
	bool isAfterOp = true;
	stack <Expression*> expressions;
	stack <char> operators;
	for (i=0;i<toShunt.length();i++){
		if (toShunt[i] == ' '){
			continue;
		}
		else if (isAfterOp && toShunt[i] == '-'){
			isNeg= true;
		}
		else if (toShunt[i] == '('){
			if (isNeg){ // if negative we multiply by -1.
				Expression * minusOne = new Num(1);
				expressions.push(minusOne);
				operators.push('*');
				isNeg = false;
			}
			operators.push(toShunt[i]);
			isAfterOp = true;
		}
		else if (isdigit(toShunt[i]) || (toShunt[i] == '.' && isdigit(toShunt[i+1]))){
			double currentValue;
			int floatCheck=0;
			int afterDot=1;
			while (i<toShunt.size() && (toShunt[i] == '.' || isdigit(toShunt[i]))){
				//exception if 2 dots in a raw.
			    if(floatCheck > 1){
			         throw runtime_error("Exception, cannot put 2 dots in a number.");
			    }
			    if (toShunt[i] == '.'){
			    	floatCheck ++;
			    	i++;
			    }
			    else if(floatCheck == 0) {
			    	currentValue = (currentValue * 10) + (toShunt[i] - '0');
			    	i++;
            	}
			    else{

			    	currentValue += (double)(toShunt[i] - '0') / (pow(10 , afterDot));
			    	i++;
			    	afterDot++;
			    }
			}
            i--;

            if (isNeg){
                currentValue *= -1;
                isNeg = false;
            }
            Expression *num = new Num(currentValue);
            currentValue = 0;
            expressions.push(num);
            isAfterOp = false;

		}
		else if (toShunt[i] == ')'){
		     while (!operators.empty() && operators.top() != '(') {
		           Expression *val2 = expressions.top();
		           expressions.pop();
		           Expression *val1 = expressions.top();
		           expressions.pop();
		           char op = operators.top();
		           operators.pop();
		           expressions.push(chooseOp(val1, val2, op));
		     }
		            // pop opening brace.
		     operators.pop();
		}
		//if its operator.
		else{
			//if 2 operators in a row.
            if(toShunt[i] == '+' && isAfterOp){
                continue;
            }
            if((toShunt[i] == '/' || toShunt [i] == '*') && isAfterOp){
                throw runtime_error("Syntax Error!");
            }
            if(expressions.empty() && (toShunt[i] == '/' || toShunt [i] == '*')){
                throw runtime_error("Syntax Error!");
            }
            while (!operators.empty() && precedence(operators.top())
                                  >= precedence(toShunt[i])) {
                Expression *val2 = expressions.top();
                expressions.pop();

                Expression *val1 = expressions.top();
                expressions.pop();

                char op = operators.top();
                operators.pop();

                expressions.push(chooseOp(val1, val2, op));
            }
            // Push current token to 'ops'.
            operators.push(toShunt[i]);
            isAfterOp = true;
		}

	}
    while(!operators.empty()){
        Expression *val2 = expressions.top();
        expressions.pop();

        Expression *val1 = expressions.top();
        expressions.pop();

        char op = operators.top();
        operators.pop();

        expressions.push(chooseOp(val1, val2, op));
    }

    // Top of 'values' contains result, return it.
    return expressions.top();
}


