#include "SocketS.hpp"

SocketS::SocketS(int p):port(p){
	sockInit();
	ft_bind();
	ft_listen();
}

SocketS::~SocketS(){
}

void SocketS::sockInit(){
	if ((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		std::cerr << "some thing goes wrong with socket";
		exit(-3);
	}
	if (!setReusFlag(serv_sock)){
		std::cerr << "Error in setting reuse flag in socket with fd" << serv_sock << "\n"; 
	}
	if (!setSocketBlocking(serv_sock)){
		std::cerr << "Error in setting reuse flag in socket with fd" << serv_sock << "\n"; 
	}
}

bool SocketS::setReusFlag(int sock){
	int flag = 1;
	socklen_t socklen = sizeof(int);
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, socklen))
		return false;
	return true;
}

bool SocketS::setSocketBlocking(int sock){
	if (fcntl(sock, F_SETFL, O_NONBLOCK))
		return false;
	return true;
}

int SocketS::ft_bind(){
	socklen_t			adr_sz;
	struct sockaddr_in	serv_adr;
	
	memset(&serv_adr,0 , sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_adr.sin_port = htons(port);
	if (bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
		std::cerr << "error in bind the addr to socket: " << serv_sock << ".\n";
		exit(2);
		return (0);
	}
	return (1);
}

int SocketS::ft_listen(){
	if (listen(serv_sock, SOMAXCONN) == -1){
		dprintf(2, "error in listen to socket.\n");
		exit(3);
		return (0);
	}
	return (1);
}

int checkForSock(std::vector<SocketS> &socs, int port){
	for (std::vector<SocketS>::iterator it = socs.begin(); it != socs.end(); it++){
		if (it->port == port)
			return it->serv_sock;
	}
	return -1;
}

void SocketSInit(std::vector<Server *> &ser){
	std::vector<SocketS> socs;
	int	ret;
	for (std::vector<Server *>::iterator it = ser.begin(); it != ser.end(); it++){
		if ((ret = checkForSock(socs, (*it)->info.port)) != -1){
			(*it)->serv_sock = ret;
			continue ;
		}
		socs.push_back(SocketS((*it)->info.port));
		(*it)->serv_sock = socs.back().serv_sock;
	}
}
