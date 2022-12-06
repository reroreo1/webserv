#ifndef SERVER_HPP
#define SERVER_HPP
#include "globalheader.hpp"
using namespace std ;

struct locations {
	std::pair<int, std::string> redirString;
	std::vector<std::string>	acceptedMeth;
	std::vector<std::string>	index;
	bool						autoIndex;
	std::string					autoIndex_form;
	std::string					root;
	std::string					dirFile;
	std::string 				locationUri;
	locations();
};
struct serverInfo {
	int							port;
	int							clientBodySize;
	std::vector<std::string>	servName;
	std::vector<std::string>	index;
	std::map<int, std::string>	errPgs;
	std::vector<locations>		locs;

	void display();
};

struct Server{
	int							serv_sock;
	serverInfo					info;
};

Server &getServer(std::vector<Server *> &ss, int servs, std::string serveN);

#endif
