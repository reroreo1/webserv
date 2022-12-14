#include "worker.hpp"

selectInfo::selectInfo(){
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	fd_max = 0;
};

selectInfo::~selectInfo(){

};

void Worker::cleaning(){ // implement l fikra
	for (int so = ss.back()->serv_sock + 1; so < (sel.fd_max + 1) / 2; so++){
		if (FD_ISSET(so, &sel.readfds))	
			FD_CLR(so, &sel.readfds);
		if (FD_ISSET(so, &sel.readfds))
			FD_CLR(so, &sel.writefds);
		close(so);
		clnt_serv.erase(so);
		req.eraseReq(so);
		res.eraseRes(so);
	}
}

void selectInfo::selectoo(){
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	cpy_rfds = readfds;
	cpy_wfds = writefds;
	ret = select(fd_max + 1, &cpy_rfds, &cpy_wfds, 0, &timeout);
}

Worker::Worker() {
	cout << "need couple parametters to work";
}

Worker::Worker(vector<Server *> &serv_sock, size_t bs): ss(serv_sock){
	buff_size = bs;
	for (vector<Server *>::iterator sock = ss.begin(); sock != ss.end(); sock++){
		FD_SET((*sock)->serv_sock, &sel.readfds);
	}
}

static int ft_memcmp(char *s1, char *s2, size_t size){
	int ret ;
	if ((ret = memcmp(s1, s2, size))){
		dprintf(2, "{s1=");bWrite(s1, size);dprintf(2, "}");
		dprintf(2, "{s2=");bWrite(s2, size);dprintf(2, "}");
	}
	return (ret);
}

int Worker::handle_response(int clnt) {
	Request &request = req.getRequest(clnt);
	char buf[buff_size];

	Server &server = getServer(ss, clnt_serv.at(clnt), getHost(request));
	Response &p = res.handleRes(clnt, request, server);
	p.fileSender();
	if (p.stat == 2){
		FD_CLR(clnt, &sel.writefds);
		if (request.getHeader(std::string("Connection")) == std::string("close") || request.getHeader(std::string("Connection")) == "NF"){
			clnt_serv.erase(clnt);
			close(clnt);
		}
		else
			FD_SET(clnt, &sel.readfds);
		res.eraseRes(clnt);
		req.eraseReq(clnt);
	}
	return 0;
}

int Worker::handle_request(char *buff, int bufLen, int clnt, int &fd_max) {
	req.handleReq(clnt, buff, bufLen);
	if (req.getRequest(clnt).fase == 2){
		FD_CLR(clnt, &sel.readfds);
		FD_SET(clnt, &sel.writefds);
	}
	return 0;
}

void Worker::start() {
	int		clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	char		buf[buff_size];
	sel.fd_max = ss.back()->serv_sock;
	while (1){
		sel.selectoo();
		if (sel.ret == -1){
			perror("selectEr: ");
			cerr << "select went wrong.\n";
			break ;
		}

		if (sel.ret == 0) {
			// continue;
			for (int so = 0; so < sel.fd_max + 1; so++){
				if (keyInMap(clnt_serv, so)){
					FD_CLR(so, &sel.readfds);
					close(so);
				}
			}
			FD_ZERO(&sel.writefds);
			clnt_serv.clear();
			req.clearReq();
			res.clearRes();
			continue ;
		}
		// connection case:

		for (int server = ss.front()->serv_sock;
			 server <= ss.back()->serv_sock; server++) {
			if (FD_ISSET(server, &sel.cpy_rfds)){ // connection request;
				adr_sz = sizeof(clnt_adr);
				clnt_sock = ft_accept(server, (struct sockaddr *) &clnt_adr, &adr_sz);
				if (clnt_sock != -1){
					fcntl(clnt_sock, F_SETFL, O_NONBLOCK);
					FD_SET(clnt_sock, &sel.readfds);
					if (sel.fd_max < clnt_sock){
						sel.fd_max = clnt_sock;
					}
					clnt_serv.insert(pair<int, int> (clnt_sock, server));
					printf("connected client: %d \n", clnt_sock);
				}
				// else add case here
			}
		}
		// reading requests
		std::cout << "serv sock : " << ss.back()->serv_sock << std::endl;
		for (int clnt = ss.back()->serv_sock + 1; clnt <= sel.fd_max; clnt++) {
			if (FD_ISSET(clnt, &sel.cpy_rfds)){
				str_len = ft_recv(clnt, buf, BUFSIZ, "recv :");
				if (str_len <= 0){ // close request
					close(clnt);
					req.eraseReq(clnt);
					res.eraseRes(clnt);
					clnt_serv.erase(clnt);
					printf("close client %d \n", clnt);
					FD_CLR(clnt, &sel.readfds);
				}
				else 
					handle_request(buf, str_len, clnt, sel.fd_max);
			}
		}
		// sending response
		for (int clnt = 0; clnt <= sel.fd_max; clnt++){
			if(FD_ISSET(clnt, &sel.cpy_wfds))
				handle_response(clnt);
		}
	}
}