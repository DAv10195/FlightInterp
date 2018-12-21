//implementation of FlightInterp class
#include "FlightInterp.h"
#include "Utils.h"
#include <map>

int main() {
	string s = "(x+3)>=2";
	map<string, double> m;
	m["x"] = 5.0;
	cout << assignVars(m, s) << endl;
	return 0;
}
