//Utilities header file
#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>
#include <map>
#include "FlightInterp.h"

using namespace std;
//decides if the inputed character is a valid operator.
bool isOper(char &c);
//same as the function before, but for a string.
bool isOper(string &s);
//decides if the inputed character is a valid digit.
bool isDig(char &c);
//check if inputed string is a number.
bool isNum(string &s);
//check if inputed string is a valid condition.
vector<string> ifCond(map<string, double>* st,string &s);
//assigns value to variables in the string. if a variable isn't declared returns empty string.
string assignVars(map<string, double>* st, string &s);
//executes the script written in the inputed file path
double execFromFile(map<string, double>* sTable, string &path, Lexer* l, Parser* p);
//thread responsible for reading data from simulator
void* readThread(void* args);
//thread responsible for writing data to the simulator.
void* writeThread(void* args);

#endif
