#include "class.hpp"

int MemeBot::Log(std::string message) {
	cout << message+"\n";
	return 0;
}

// Not my code, this was stolen from Evan Teran from stackoverflow
// http://stackoverflow.com/questions/236129/split-a-string-in-c

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void trim(std::string& s) {
    while(s.compare(0,1," ")==0)
        s.erase(s.begin()); // remove leading whitespaces
    while(s.size()>0 && s.compare(s.size()-1,1," ")==0)
        s.erase(s.end()-1); // remove trailing whitespaces
}

void irc_trim(std::string& s) {
    while(s.size()>0 && s.compare(s.size()-1,1,"\n")==0)
        s.erase(s.end()-1); // remove trailing newline
    while(s.size()>0 && s.compare(s.size()-1,1,"\r")==0)
        s.erase(s.end()-1); // remove trailing carriage return
}
