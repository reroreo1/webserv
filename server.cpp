#include "server.hpp"

locations::locations(): redirString(std::pair<int, std::string>(-1, "")), autoIndex(false),
	autoIndex_form("html"), root(""), locationUri(""), dirFile(""){}

void serverInfo::display(){
	std::cerr << "max client body size :" << clientBodySize << "\n";

	std::cerr << "server_name :";
	for (std::vector<std::string>::iterator it = servName.begin(); it != servName.end(); it++){
		std::cerr << *it << " ";
	}
	std::cerr << std::endl;
	
	std::cerr << "port :" << port << "\n";
	
	std::cerr << "index :";
	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++){
		std::cerr << *it << " ";
	}
	std::cerr << std::endl;

	std::cerr << "erroPgs :";
	for (std::map<int, std::string>::iterator it = errPgs.begin(); it != errPgs.end(); it++){
		std::cerr << (*it).first << ":" << (*it).second << " ";
	}
	std::cerr << std::endl;

	std::cerr << "locations :";
	for (std::vector<locations>::iterator it = locs.begin(); it != locs.end(); it++){
		std::cerr << it->locationUri << std::endl;
		std::cerr << it->root << std::endl;
		std::cerr << (it->autoIndex ? "on" : "off") << std::endl;
		std::cerr << it->autoIndex_form << std::endl;
		std::cerr << it->redirString.first << " : " << it->redirString.second << "\n";
		for (std::vector<string>::iterator m = it->acceptedMeth.begin(); m != it->acceptedMeth.end() ; m++)
			std::cerr << *m << " ";
		std::cerr << std::endl;	
		std::cerr << "indexes :";
		for (std::vector<string>::iterator m = it->index.begin(); m != it->index.end() ; m++)
			std::cerr << *m << " ";
		cerr << endl;
		std::cerr << it->dirFile << "\n";
	}
}

static bool containServNam(std::vector<string> &s, std::string servName){
	for(std::vector<string>::iterator it = s.begin(); it != s.end(); it++){
		if (*it == servName)
			return true;
	}
	return false;
}

Server &getServer(std::vector<Server *> &ss, int servs, std::string serveN){
	std::vector<Server *>::iterator tmp;
	for (std::vector<Server *>::iterator it = ss.begin(); it != ss.end(); it++){
		if 	((*it)->serv_sock == servs){
			tmp = it;
			if (containServNam((*it)->info.servName, serveN))
				return **it;
		}
	}
	return **tmp;
}