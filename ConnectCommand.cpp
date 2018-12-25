//Implementation of ConnectCommand object
#include "Command.h"
#include "sys/socket.h"
#include <string.h>
#include "netinet/in.h"
#include <netdb.h>
#include "Utils.h"
#define SUCCESS 0
#define FAIL 1
#define SOCKFD 2
#define W_THREAD 1
#define IP_NUMS 4
#define DOT_NUM 3
#define MAX_IP 255
//check if valid i.p.
bool ifValidIP(string &s)
{	//i.p. can't start with a dot
	if (s.at(0) == '.')
	{
		return false;
	}
	unsigned int i = 0, size = s.size(), dotCount = 0;
	string num = "";
	vector<string> nums;
	for (; i < size; i++)
	{	//not a digit or a '.'
		if (!isDig(s.at(i)) && s.at(i) != '.')
		{
			return false;
		}
		if (s.at(i) == '.')
		{	//can't have two dots in a row
			if (s.at(i - 1) == '.')
			{
				return false;
			}
			dotCount++;
			continue;
		}
		while (i < size && isDig(s.at(i)))
		{
			num.push_back(s.at(i));
			i++;
		}
		i--;
		nums.push_back(num);
		num = "";
	}
	size = nums.size();
	if ((size != IP_NUMS) || (dotCount != DOT_NUM))
	{
		return false;
	}
	for (i = 0; i < size; i++)
	{	//no need to check if less than zero because we already checked for '-'
		if (stoi(nums[i]) > MAX_IP)
		{
			return false;
		}
	}
	return true;
}
//execute method
double ConnectCommand :: execute()
{	//case not enough params passed
	if (this->ind == (this->params).size() - 1 || this->ind == (this->params).size() - 2)
	{
		cout << "invalid paramaters passed to connect function" << endl;
		return FAIL;
	}
	struct sockaddr_in serv_adr;
	struct hostent* server;
	// first parameter is the ip.
	string portStr = this->params[this->ind + 2];
	string ip = this->params[this->ind + 1];
	if (!ifValidIP(ip))
	{
		cout << "invalid I.P. address" << endl;
		return FAIL;
	}
	int port = 0;
	Expression* e1 = nullptr;
	//handle the port, it could be an expression.
	portStr = assignVars((this->tAl)->lock, this->sTable ,portStr);
	if (portStr == "")
	{
		cout << "invalid paramaters passed to connect function" << endl;
		return FAIL;
	}
	(this->sy)->SetExpToShunt(portStr);
	try
	{
		e1 = this->sy->Shunt();
		if (e1 == nullptr)
		{
			cout << "invalid paramaters passed to connect function" << endl;
			return FAIL;
		}
		//get the port value.
		port = e1->calculate();
		delete e1;
	}
	catch(runtime_error &e)
	{
		delete e1;
		cout << e.what() << endl;
		return FAIL;
	}
	//create new socket.
	this->socketId[SOCKFD] = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketId[SOCKFD] < 0)
	{ //if creation failed.
		cout << "Connection error" << endl;
		return FAIL;
	}
	//get the host by ip.
	server = gethostbyname(ip.c_str());
	if (server == NULL)
	{ // if we failed create the server.
		cout << "Error with server connection" << endl;
		return FAIL;
	}
	// initialize to zero and copy the data to the address struct.
	bzero((char *) &serv_adr, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_adr.sin_addr.s_addr, server->h_length);
	serv_adr.sin_port = htons(port);
	if (connect(this->socketId[SOCKFD], (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
	{ //connect to the socket.
		perror("Error: ");
		cout << "Connection error" << endl;
		return FAIL;
	}
	return 0;
}





