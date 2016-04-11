#include "class.hpp"
#include <unistd.h>

int MemeBot::Connect() {
	struct addrinfo hints,*servinfo;
	memset(&hints, 0, sizeof hints);
//	This is from the Twitch developer site
	hints.ai_family = AF_INET;
	hints.ai_family = AF_UNSPEC;	
	hints.ai_socktype = SOCK_STREAM; 
//	String to char
	const char* server = MemeBot::server.c_str();
	const char* port   = MemeBot::port.c_str();
//	Fetch server info for socket
	int err;
	err = getaddrinfo(server,port,&hints, &servinfo);
	if (err != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
	} else {
		MemeBot::Log("Succesfully retrieved server info");
	}
//	Create socket
	MemeBot::sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (MemeBot::sock==-1) 
	{
		MemeBot::Log("Failed to create socket");
	} else {
		MemeBot::Log("Socket has been initialized succesfully");
	}
//	Connect socket
	err = connect(MemeBot::sock, servinfo->ai_addr, servinfo->ai_addrlen);
	if (err == -1)
	{
		MemeBot::Log("Socket connection failed");
	} else {
		MemeBot::Log("Connected succesfully to server");
	}
//	Clean and return
        freeaddrinfo(servinfo);
	return 0;
}

int MemeBot::Login() {
	MemeBot::Log("Sending login credentials");	
	MemeBot::Speak("PASS "+MemeBot::oauth);
	MemeBot::Speak("NICK "+MemeBot::name);	
	MemeBot::Speak("USER "+MemeBot::name+" * 8:"+MemeBot::gecos);	
	MemeBot::Log("Waiting for MotD to be over, checking login");
	bool motd = true;
	bool cool = true;
	int imin,imax,i,vsize;
	while(motd) {
		MemeBot::Listen();
		imin = 0;
		imax = MemeBot::vecdata.size();
		for (i=imin;i<imax;i++) {
			irc_trim(MemeBot::vecdata[i]);
			MemeBot::Log(">"+MemeBot::vecdata[i]);	
			MemeBot::vecline = split(MemeBot::vecdata[i],' ');
			vsize = MemeBot::vecline.size();
			if (vsize>=2) {
				if (MemeBot::vecline[1]=="376") {
					MemeBot::Log("Found EOM flag");
					motd = false;
				}
			}
			if (vsize>=3) {
				if (MemeBot::vecline[1]=="NOTICE") {
					MemeBot::Log("Error logging in");
					motd = false;
					cool = false;
				}
			}
		}
	}
	if (cool) {
		MemeBot::Log("MotD is over");
	}
//	Twitch requires this line for some commands, hell if I know what it is
	MemeBot::Speak("CAP REQ :twitch.tv/commands");
	MemeBot::Listen();
	imax = MemeBot::vecdata.size();
	for (i=imin;i<imax;i++) {
		irc_trim(MemeBot::vecdata[i]);
		MemeBot::Log(">"+MemeBot::vecdata[i]);	
		MemeBot::vecline = split(MemeBot::vecdata[i],' ');
		vsize = MemeBot::vecline.size();
	}
//	If the last step was succesful we can join a channel
	if (MemeBot::vecline[1]=="CAP") {
		MemeBot::Speak("JOIN "+MemeBot::channel);
	}
	MemeBot::Listen();
	imax = MemeBot::vecdata.size();
	for (i=imin;i<imax;i++) {
		irc_trim(MemeBot::vecdata[i]);
		MemeBot::Log(">"+MemeBot::vecdata[i]);	
		MemeBot::vecline = split(MemeBot::vecdata[i],' ');
		vsize = MemeBot::vecline.size();
	}
	return 0;
}

int MemeBot::Listen() {
//	Fetch raw data, it has /r/n all over the place
	memset(MemeBot::rawdata,0,MAXDATASIZE);
	int numbytes = recv(MemeBot::sock,MemeBot::rawdata,MAXDATASIZE-1,0);
	MemeBot::strdata = MemeBot::rawdata;
//	Divide by lines, this get rid of /n
	istringstream sline;
	std::string line;
	sline.str(MemeBot::strdata);
	char delim;
	delim = '\n';
	MemeBot::vecdata = split(MemeBot::strdata,delim);
	return 0;
}

int MemeBot::Speak(std::string message) {
	if (MemeBot::verbose) 
		MemeBot::Log("<"+message);
	std::string dum = message+"\r\n";
	const char* msg = dum.c_str();
	int len = strlen(msg);
	int bytes_sent = send(MemeBot::sock, msg, len, NULL);
	if (bytes_sent == 0)	{
		MemeBot::Log("Empty message was sent");	
		return false;
	}
	usleep(MemeBot::cooldown);
	return 0;
}
