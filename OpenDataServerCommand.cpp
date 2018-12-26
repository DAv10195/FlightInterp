//Implementation of Open DataServerCommand
#include "Command.h"
#include "Utils.h"
#include "FlightInterp.h"
#include "sys/socket.h"
#include <string.h>
#include "netinet/in.h"
#define SUCCESS 0
#define FAIL 1
#define READ_THREAD 1
#define SOCKFD 0
#define NSOCK 1
#define R_THREAD 0
#define MAX_CONNECTIONS 5
//execute method
double OpenDataServerCommand :: execute()
{
	Expression* e1 = nullptr, * e2 = nullptr;
	int port = 0, hz = 0, clilen = 0;
	//structs for storing the connection information.
	struct sockaddr_in serv_adr,cli_adr;
	//case not enough params passed
	if (this->ind == (this->params).size() - 1 || this->ind == (this->params).size() - 2)
	{
		cout << "invalid paramaters passed to openDataServer function" << endl;
		return FAIL;
	}
	string param1 = this->params[this->ind + 1], param2 = this->params[this->ind + 2];
	this->socketId[SOCKFD] = socket(AF_INET,SOCK_STREAM,0); // create new socket.
	//case initialization went wrong.
	if (this->socketId[SOCKFD] < 0)
	{
		cout << "Error with initialization of socket" << endl;
		return FAIL;
	}
	bzero((char*) &serv_adr,sizeof(serv_adr)); // put zeros in.
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = INADDR_ANY;
	//handaling port
	param1 = assignVars((this->tAl)->lock, this->sTable, param1);
	param2 = assignVars((this->tAl)->lock, this->sTable, param2);
	if (param1 == "" || param2 == "")
	{
		cout << "invalid paramaters passed to openDataServer function" << endl;
		return FAIL;
	}
	(this->sy)->SetExpToShunt(param1);
	try
	{
		e1 = this->sy->Shunt();
		if (e1 == nullptr)
		{
			cout << "invalid paramaters passed to openDataServer function" << endl;
			return FAIL;
		}
		port = e1->calculate();
		delete e1;
	}
	catch(runtime_error &e)
	{
		delete e1;
		cout << e.what() <<endl;
		return FAIL;
	}
	serv_adr.sin_port = htons(port); // initialize port.
	//handaling hz
	(this->sy)->SetExpToShunt(param2);
	try
	{
		e2 = this->sy->Shunt();
		if (e2 == nullptr)
		{
			cout << "invalid paramaters passed to openDataServer function" <<endl;
			return FAIL;
		}
		hz = e2->calculate();
		delete e2;
	}
	catch(runtime_error &e)
	{
		delete e2;
		cout << e.what() <<endl;
		return FAIL;
	}
	//bind the socket with the address.
	 if (bind(this->socketId[SOCKFD], (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
	 {
	      cout << "Connection Error: binding failed." << endl;
	      cout << "Try lunching FlightGear with the appropriate settings after lunching this interpreter" << endl;
	      return FAIL;
	 }
	 //listen for connection.
	 if (listen(socketId[SOCKFD],MAX_CONNECTIONS) < 0)
	 {
		 cout << "Connection Error" << endl;
		 return FAIL;
	 }
	 //handle the client address in order to establish connection.
	 clilen = sizeof(cli_adr);
	 // Accept actual connection from the client.
	 this->socketId[NSOCK] = accept(this->socketId[SOCKFD], (struct sockaddr *)&cli_adr, (socklen_t*)&clilen);
	 if (this->socketId[NSOCK] < 0)
	 {
	     cout << "Connection Error" << endl;
	     return FAIL;
	 }
	 //construct params to thread
	 threadParams* p = new threadParams();
	 pthread_mutex_t* lock = (this->tAl)->lock;
	 bool connection = false;
	 p->sTable = this->sTable;
	 p->refs = this->refs;
	 p->ifRun = this->ifRun;
	 p->connection = &connection;
	 p->lock = (this->tAl)->lock;
	 p->sockfd = this->socketId[NSOCK];
	 p->hz = hz;
	 //start the thread and handle case of failure
	 if (pthread_create((this->tAl)->thread, NULL, &readThread, (void*)p))
	 {
		 delete p;
		 cout << "Threading Error" << endl;
		 return FAIL;
	 }
	 *this->ifCreated = true;
	 bool test = false;
	 while (!test)
	 {	//wait for first input from FlightGear
		 pthread_mutex_lock(lock);

		 test = connection;

		 pthread_mutex_unlock(lock);
	 }
	 return SUCCESS;
}




