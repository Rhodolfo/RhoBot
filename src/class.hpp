#include <netdb.h>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#define MAXDATASIZE 330 // 154*2 + 28 I saw
using namespace std;
class MemeBot {
	public:
		std::string server;
		std::string port;
		std::string name;
		std::string oauth;
		std::string channel;
		std::string owner;
		std::string gecos;
		std::string clndata;
		std::string strdata;	
		std::string strline;	
		std::vector<std::string> vecdata;
		std::vector<std::string> vecline;	
		char rawdata[MAXDATASIZE];
		int sock;
		bool verbose;
		int  cooldown;
//		Initialize
		int Init();
//		Socket handling
		int Connect();
		int Login();
		int Listen();
		int Speak(std::string message);
//		Chat line parsing
		int Lurk();
		int Chat();
		int Pong();
		int ParseLine();
		int Respond(std::string message);
//		IO
		int Log(std::string message);
};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void trim(string& s);
void irc_trim(string& s);
