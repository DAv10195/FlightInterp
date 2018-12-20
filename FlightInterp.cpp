//implementation of FlightInterp class
#include "FlightInterp.h"

int main() {
	string s = "OpenDataServer { (50+4)*100 (5+2) 	/ 7 }";
	Lexer* l = new Lexer();
	l->setStrToLex(s);
	vector<string> v = l->lex();
	vector<string>& p = v;
	for (string str : p)
	{
		cout << str << endl;
	}
	Expression* e = new Div(new Num(20), new Num(5));
	try
	{
		cout << e->calculate();
	}
	catch (runtime_error &e)
	{
		cout << e.what() << endl;
	}
	delete e;
	delete l;
	return 0;
}
