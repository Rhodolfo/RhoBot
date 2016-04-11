#include "class.hpp"

int MemeBot::Lurk() {
	int imax,imin,i,vsize;
	imin = 0;
	MemeBot::Respond("Not to worry, the memes are here!");
	while (1) {
		MemeBot::Listen();
		imax = MemeBot::vecdata.size();
		for (i=imin;i<imax;i++) {
			irc_trim(MemeBot::vecdata[i]);
			MemeBot::strline = MemeBot::vecdata[i];
			MemeBot::vecline = split(MemeBot::vecdata[i],' ');
			MemeBot::Log(">"+MemeBot::vecdata[i]);
			MemeBot::ParseLine();
		}
	}
}

int MemeBot::ParseLine() {
	int vsize;
	vsize = MemeBot::vecline.size();
	if (vsize>=1) {
		if (MemeBot::vecline[0]=="PING") {MemeBot::Pong();}
		if (MemeBot::vecline[1]=="PRIVMSG") {MemeBot::Chat();}
	}
	return 0;
}

int MemeBot::Pong() {
	MemeBot::Speak("PONG "+MemeBot::vecline[1]);
	return 0;
}

int MemeBot::Chat() {
	int vsize,count;
	std::string user,firstword,message;
	std::vector<std::string> unamefull,uname;
	vsize = MemeBot::vecline.size();
	if (vsize>=4) {
//		Extract username
		unamefull = split(MemeBot::vecline[0],'!');
		uname     = split(unamefull[0],':');
		user      = uname[1];
//		Extract first word of message, remove leading ':'
		firstword = vecline[3];
		while(firstword.compare(0,1,":")==0) 
        		firstword.erase(firstword.begin());
		trim(firstword);
//		Extract rest of message, let's optimize operations by using leading array elements
		count = MemeBot::vecline[0].size() + 1 +
			MemeBot::vecline[1].size() + 1 +
			MemeBot::vecline[2].size() + 1 +
			MemeBot::vecline[3].size();
		message = MemeBot::strline;
		message.erase(0,count);
//		Now we have the user, the first word 'firstword', and the reest of the message 'message'
		MemeBot::Log("user: "+user+" message: "+firstword+message);
		if (firstword=="wiggle") {
			MemeBot::Respond("one two");
		}
	}
}

int MemeBot::Respond(std::string msg) {
	MemeBot::Speak("PRIVMSG "+MemeBot::channel+" :"+msg);
}

/*
[message in madmikegamerxl1's channel]
twitchnotify: nate just subscribed!
twitchnotify: nate subscribed for 13 months in a row!
twitchnotify: 13 viewers resubscribed while you were away!

[message in channels hosting madmikegamerxl1's channel]
twitchnotify: nate just subscribed to madmikegamerxl1!
twitchnotify: nate subscribed to madmikegamerxl1 for 13 months in a row!
*/
