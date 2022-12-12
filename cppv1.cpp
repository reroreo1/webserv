#include "globalheader.hpp"
#include "worker.hpp"
#include "confFile.hpp"
#include "SocketS.hpp"
using namespace std;

int main(int ac, char **av){  
	signal(SIGPIPE, SIG_IGN);
	std::vector<Server *> sv;
	ConfParse conf(sv);
	conf.confParse(std::string(av[1]));
	SocketSInit(sv);
	Worker ser(sv, BUFSIZE);
	ser.start();
	return 0;
}