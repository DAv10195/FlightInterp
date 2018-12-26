//implementation of utility functions.
#include "Utils.h"
#include "FlightInterp.h"
#include <fstream>
#include <string.h>
#define COND_OPERANDS 3
#define FIRST_OP 0
#define SECOND_OP 2
#define FAIL 1
#define BUFFER_SIZE 1024
#define PATHES 23

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
//an Empty vector indicates an invalid condition. otherwise, the condition is valid.
vector<string> ifCond(pthread_mutex_t* lock, map<string, double>* st,string &s)
{
	char c = 0;
	unsigned int i = 0;
	unsigned int size = s.size();
	string str = "";
	vector<string> toRet;
	string op = "";
	while (i < size)
	{	//build operands
		while (i < size && s.at(i) != '!' && s.at(i) != '<' && s.at(i) != '>' && s.at(i) != '=')
		{	//ignore tokens
			if (s.at(i) == '(' || s.at(i) == ')' || s.at(i) == ' ')
			{
				i++;
				continue;
			}
			str.push_back(s.at(i));
			i++;
		}
		if (str == "")
		{	//not enough operands...
			toRet.clear();
		}
		toRet.push_back(str);
		//case an operator was encountered
		if (i < size && (s.at(i) == '<' || s.at(i) == '>' || s.at(i) == '=' || s.at(i) == '!'))
		{
			if (op != "")
			{	//case another operator was read...
				toRet.clear();
				break;
			}
			c = s.at(i);
			if (c == '<')
			{	//case <=
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = "<=";
					op = str;
					toRet.push_back(op);
					str = "";
					i++;
				}	//case <
				else
				{
					str = "<";
					op = str;
					toRet.push_back(op);
					str = "";
				}
			}
			if (c == '>')
			{	//case >=
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = ">=";
					op = str;
					toRet.push_back(op);
					str = "";

					i++;
				}	//case >
				else
				{
					str = ">";
					op = str;
					toRet.push_back(op);
					str = "";
				}
			}
			if (c == '!')
			{	//case !=
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = "!=";
					op = str;
					toRet.push_back(op);
					str = "";
					i++;
				}	//case ! not followed by =
				else
				{
					toRet.clear();
					break;
				}
			}
			if (c == '=')
			{	//case ==
				if (i + 1 < size && s.at(i + 1) == '=')
				{
					str = "==";
					op = str;
					toRet.push_back(op);
					str = "";
					i++;
				}	//case = not followed by =
				else
				{
					toRet.clear();
					break;
				}
			}
		}
		i++;
	}
	//case num of operands is not 2 or no operator was read
	if (op == "" || toRet.size() != COND_OPERANDS)
	{
		toRet.clear();
	}

	pthread_mutex_lock(lock);

	//case first operand is not a number and an undeclared variable
	if (!isNum(toRet[FIRST_OP]) && st->find(toRet[FIRST_OP]) == st->end())
	{
		cout << "var " << toRet[FIRST_OP] << " undeclared" << endl;
		toRet.clear();
	}
	//case second operand is not a number and an undeclared variable
	if (!isNum(toRet[SECOND_OP]) && st->find(toRet[SECOND_OP]) == st->end())
	{
		cout << "var " << toRet[SECOND_OP] << " undeclared" << endl;
		toRet.clear();
	}

	pthread_mutex_unlock(lock);

	return toRet;
}
//assigns value to variables in the string. if a variable isn't declared returns empty string.
string assignVars(pthread_mutex_t* lock, map<string, double>* st, string &s)
{
	string toRet = "";
	string var = "";
	double val = 0;
	unsigned int i = 0;
	unsigned int size = s.size();

	for (; i < size; i++)
	{	//a digit or an operator is good
		if (isOper(s.at(i)) || isDig(s.at(i)) || s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r' || s.at(i) == '.')
		{	//ignore whitespaces
			if (s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r')
			{
				continue;
			}

			toRet.push_back(s.at(i));
		}
		else
		{	//a variable has been encountered
			while (i < size && !isOper(s.at(i)))
			{	//build the name of the variable, ignoring whitespaces
				if (s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r')
				{
					i++;
					continue;
				}
				var.push_back(s.at(i));
				i++;
			}
			i--;

			pthread_mutex_lock(lock);

			//case variable hasn't been declared;
			if (st->find(var) == st->end())
			{
				pthread_mutex_unlock(lock);
				return "";
			}
			//assign value instead of the variable to the returned string.
			val = st->at(var);

			pthread_mutex_unlock(lock);

			var = "";
			toRet += to_string(val);
		}
	}

	return toRet;
}
//executes the script written in the inputed file path
double execFromFile(map<string, double>* sTable, map<string, string>* refs, map<string, string>* revRefs,
string &path, Lexer* l, Parser* p)
{
	string line = "";
	vector<string> toParse;
	vector<string> fromLexer;
	ifstream f;
	unsigned int size = 0, i = 0;

	f.open(path);
	if (!f)
	{	//invalid path...
		cout << "Invalid file path was passed" << endl;
		return FAIL;
	}
	else
	{
		while (getline(f, line))
		{	//lex all of the script in the file
			l->setStrToLex(line);
			fromLexer = l->lex();
			size = fromLexer.size();
			for (; i < size; i++)
			{
				toParse.push_back(fromLexer[i]);
			}
			i = 0;
		}
		f.close();
		p->setVecToParse(toParse);
		return p->Parse(sTable, refs, revRefs);
	}
}
//initialize path array for readThread
void initPathes(string* arr)
{
	arr[0] = "/instrumentation/airspeed-indicator/indicated-speed-kt";
	arr[1] = "/instrumentation/altimeter/indicated-altitude-ft";
	arr[2] = "/instrumentation/altimeter/pressure-alt-ft";
	arr[3] = "/instrumentation/attitude-indicator/indicated-pitch-deg";
	arr[4] = "/instrumentation/attitude-indicator/indicated-roll-deg";
	arr[5] = "/instrumentation/attitude-indicator/internal-pitch-deg";
	arr[6] = "/instrumentation/attitude-indicator/internal-roll-deg";
	arr[7] = "/instrumentation/encoder/indicated-altitude-ft";
	arr[8] = "/instrumentation/encoder/pressure-alt-ft";
	arr[9] = "/instrumentation/gps/indicated-altitude-ft";
	arr[10] = "/instrumentation/gps/indicated-ground-speed-kt";
	arr[11] = "/instrumentation/gps/indicated-vertical-speed";
	arr[12] = "/instrumentation/heading-indicator/indicated-heading-deg";
	arr[13] = "/instrumentation/magnetic-compass/indicated-heading-deg";
	arr[14] = "/instrumentation/slip-skid-ball/indicated-slip-skid";
	arr[15] = "/instrumentation/turn-indicator/indicated-turn-rate";
	arr[16] = "/instrumentation/vertical-speed-indicator/indicated-speed-fpm";
	arr[17] = "/controls/flight/aileron";
	arr[18] = "/controls/flight/elevator";
	arr[19] = "/controls/flight/rudder";
	arr[20] = "/controls/flight/flaps";
	arr[21] = "/controls/engines/current-engine/throttle";
	arr[PATHES - 1] = "/engines/engine/rpm";
}
//thread responsible for reading data from simulator
void* readThread(void* args)
{	//extracting paramaters
	threadParams* p = (threadParams*) args;
	pthread_mutex_t* lock = p->lock;
	map<string, string>* refs = p->refs;
	map<string, double>* sTable = p->sTable;
	int sockfd = p->sockfd;
	bool run = *(p->ifRun);
	int status = 0, i = 0, j = 0, size = 0;
	char buffer[BUFFER_SIZE];
	string val = "", ref = "", var = "", leftovers = "", message = "";
	string pathes[PATHES];
	double vals[PATHES];	//initialize values to 0
	for (; i < PATHES; i++)
	{
		vals[i] = 0;
	}
	i = 0;
	initPathes(pathes);

	while (run)
	{	//read data
		bzero(buffer, BUFFER_SIZE);
		status = read(sockfd, buffer, BUFFER_SIZE - 1);
		//case reading failed
		if (status < 0)
		{
			cout << "Error reading data from FlightGear" << endl;
			delete p;
			return NULL;
		}
		//store data received
		while (buffer[i] && buffer[i] != '\n')
		{
			message.push_back(buffer[i]);
			i++;
		}
		size = message.size();
		i = 0;
		while (j < PATHES && i < size)
		{
			while (i < size && (isDig(message.at(i)) || message.at(i) == '-' || message.at(i) == '.'))
			{
				val.push_back(message.at(i));
				i++;
			}
			vals[j] = stod(val);
			val = "";
			i++;
			j++;
		}
		j = 0;
		i = 0;
		message = "";
		//update sTable according to data received
		pthread_mutex_lock(lock);

		while (j < PATHES)
		{
			ref = pathes[j];
			while (refs->find(ref) != refs->end())
			{
				var = (*refs)[ref];
				(*sTable)[var] = vals[j];
				ref = var;
			}
			j++;
		}
		j = 0;

		pthread_mutex_unlock(lock);
		//check if still needed to run
		pthread_mutex_lock(lock);

		run = *(p->ifRun);

		pthread_mutex_unlock(lock);

	}

	delete p;
	return NULL;
}
