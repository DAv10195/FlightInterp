//implementation of FlightInterp class
#include "FlightInterp.h"
#include "Utils.h"
#include <map>



int main() {
	string s = "dkgjdkjdgj/sd//";
	ShuntingYarder* sy = new ShuntingYarder();
	sy->SetExpToShunt(s);
	Expression* e = sy->Shunt();
	if (e == nullptr)
	{
		cout << "bla";
	}
	return 0;
}
