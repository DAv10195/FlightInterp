//Implementation of ConnectCommand object
#include "Command.h"
#define SUCCESS 0
#define FAIL 1
//execute method
double ConnectCommand :: execute()
{
	// first parameter is the ip.
	string ip = this->params[this->ind+1];
	int port;
	Expression * e1 = nullptr;
	//handle the port, it could be an expression.
	(this->sy)->SetExpToShunt(this->params[this->ind+2]);
	try
	{
		e1 = this->sy->Shunt();
		if (e1 == nullptr)
		{
			cout << "invalid paramaters passed to openDataServer function" <<endl;
			return FAIL;
		}
		//get the port value.
		port = e1->calculate();
		delete e1;
	}
	catch(runtime_error &e)
	{
		delete e1;
		cout << e.what() <<endl;
		return FAIL;
	}

	return 0;
}




