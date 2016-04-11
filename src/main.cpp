#include "class.hpp"

int main() {
//	Initialize bot
	MemeBot RhoBot;
	RhoBot.Init();
	RhoBot.Connect();
	RhoBot.Login();
	RhoBot.Lurk();
	return 0;
}
