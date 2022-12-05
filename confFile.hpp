#ifndef CONFFILE_HPP
#define CONFFILE_HPP
#include "globalheader.hpp"
#include "server.hpp"
#define TOKENSIZE 12

using namespace std;

struct ConfParse {

	std::ifstream conf;
	std::string buff;
	std::vector<std::string> tokens;
	std::vector<Server *> &srvs;

	char delTable[3][2];// = {";", "{", "}"};
	char specToken[TOKENSIZE][30];
	char autoIndForm[4][10];
	char accMethd[3][10];
	enum {cmbs,liste, servname, root, index, errp, loc, autoin, autoinfor, red, accmet, dirfile};
	enum {html, xml, json, jsonp};

	template <typename T>
	int indInTable(std::string Tok, T table, int size){
		for (int i = 0; i < size; i++){
		if (table[i] == Tok)
			return i;
		}
		return -1;
	}
	
	ConfParse(std::vector<Server *> &ss);

	int specDel(char c);

	void worker();
	
	void errorParse(std::string error);

	void listenParse(std::vector<std::string>::iterator &it, Server *sok);

	void clientMaxParse(std::vector<std::string>::iterator &it, Server *sok);
	
	void indexParse(std::vector<std::string>::iterator &it, Server *sok);

	void indexParse(std::vector<std::string>::iterator &it, locations &loc);

	void servNameParse(std::vector<std::string>::iterator &it, Server *sok);

	// need more protectio
	void errpParse(std::vector<std::string>::iterator &it, Server *sok);

	void autoinParse(std::vector<std::string>::iterator &it, locations &loc);

	void autoinforParse(std::vector<std::string>::iterator &it, locations &loc);

	void rootParse(std::vector<std::string>::iterator &it, locations &loc);
	
	void redParse(std::vector<std::string>::iterator &it, locations &loc);

	void accMethParse(std::vector<std::string>::iterator &it, locations &loc);

	void dirFileParse(std::vector<std::string>::iterator &it, locations &loc);

	void locParse(std::vector<std::string>::iterator &it, Server *sok);

	void parseserv(std::vector<std::string>::iterator &it);

	void parser();

	void tokenize(std::ifstream &conf);

	void confParse(std::string confName);
};

#endif

// int main(int ac, char **av){
// 	std::vector<Server *> sv;
// 	ConfParse pars(sv);
// 	int num;

// 	pars.confParse(std::string(av[1]));
// 	for (auto sok : sv){
// 		std::cerr << "-------------" << num++ << "-------------" << '\n'; 
// 		sok->info.display();
// 	}
// 	return 0;
// }