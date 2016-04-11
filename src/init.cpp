#include "class.hpp"

int MemeBot::Init() {
	MemeBot::verbose  = true;
	MemeBot::cooldown = 1500000;
	bool x;
	std::string var;
	std::string val;
	std::string line;
	std::string cline;
	std::string dummy;	
	std::ifstream file("login.h");
	std::vector<std::string> vline;
	while (std::getline(file,line)) {
		std::istringstream sline;
		sline.str(line);
		vline = split(line,'\/\/');
		cline = vline[0];	
		vline = split(cline, '=');
		switch (vline.size()) {
		case 2:
			var = vline[0];
			val = vline[1];
			trim(var);
			trim(val);
			if (var=="server") {
				MemeBot::Log("server   = "+val);
				MemeBot::server = val;
			} else if (var=="port") {
				MemeBot::Log("port     = "+val);
				MemeBot::port = val;
			} else if (var=="name") {
				MemeBot::Log("name     = "+val);
				MemeBot::name = val;
			} else if (var=="channel") {
				MemeBot::Log("channel  = "+val);
				MemeBot::channel = val;
			} else if (var=="owner") {
				MemeBot::Log("owner    = "+val);
				MemeBot::owner = val;
			} else if (var=="gecos") {
				MemeBot::Log("gecos    = "+val);
				MemeBot::gecos = val;
			} else if (var=="oauth") {
				MemeBot::Log("oauth    = "+val);
				MemeBot::oauth = val;
			} else if (var=="cooldown") {
				MemeBot::Log("cooldown = "+val);
//				MemeBot::cooldown = stoi(val);
			}
		}
	}
	return 0;
}
