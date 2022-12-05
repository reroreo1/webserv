#ifndef SOCKETS_HPP
#define SOCKETS_HPP
#include "globalheader.hpp"
#include "server.hpp"
struct SocketS{
	int serv_sock;
	int port;

	SocketS(int p);

	void sockInit();

	bool setReusFlag(int sock);

	bool setSocketBlocking(int sock);

	int ft_bind();

	int ft_listen();

	~SocketS();
};

void SocketSInit(std::vector<Server *> &ser);
int checkForSock(std::vector<SocketS> &socs, int port);

#endif