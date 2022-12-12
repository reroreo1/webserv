#include "worker.hpp"

selectInfo::selectInfo(){
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	fd_max = 0;
};

selectInfo::~selectInfo(){

};

void selectInfo::selectoo(){
	timeout.tv_sec = 1;
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
		// cerr << "sock->serv_sock :: " << sock->serv_sock;
		FD_SET((*sock)->serv_sock, &sel.readfds);
	}
}

int Worker::handle_request(char *buff, int bufLen, int i, int &fd_max) {
	req.handleReq(i, buff, bufLen, &sel.writefds);
	if (req.getRequest(i).fase == 2){
		FD_SET(i, &sel.writefds);
		if (fd_max < i)
			fd_max = i;
	}
	return 0;
}



int Worker::handle_response(int clnt) {
	
	Request &request = req.getRequest(clnt);
	char buf[buff_size];
	// std::cout << "hello hello" << getHost(request) << std::endl;
	if (request.fase == 2){
		std::cerr << "--------------------\n";
		Server &server = getServer(ss, clnt_serv.at(clnt), getHost(request));
		Response &p = res.handleRes(clnt, request, server);
		p.display();
		p.uri.displayUri();

		std::cerr << "--------------------\n";
		p.fileSender();
		
		// test
		// {
		// 	int hd = ft_open("./helperFile/header", O_RDONLY);
		// 	int bd = ft_open("./helperFile/h.html", O_RDONLY);
		// 	int ret;
		// 	ret = ft_read(hd, buf, BUFSIZ, "ftread");
		// 	memmove(buf + ret,(char *) "\r\n\r\n",  4); ret += 4;
		// 	ret += ft_read(bd, buf + ret, BUFSIZ - ret, "ftread2");
		// 	ft_send(clnt, buf, ret, "here");
		// 	// int t2 = ft_open("testingFiles/t2", O_WRONLY | O_TRUNC);
		// 	// std::cerr << "ret from test2:" << ret;
		// 	// ft_write(t2, buf, ret, "filesSender.");
		// 	// close(t2);
		// 	close(hd);
		// 	close(bd);
		// }
		// exit(1);
		if (true){
			FD_CLR(clnt, &sel.writefds);
			res.eraseRes(clnt);
			if (request.getHeader(std::string("Connection")) == std::string("close")){
				req.eraseReq(clnt);
				FD_CLR(clnt, &sel.readfds);
				clnt_serv.erase(clnt);
				close(clnt);
				std::cerr << "closing connection from res.\n";
			}
		}
	}
	return 0;
}

void Worker::start() {
	int		clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	char		buf[buff_size];
	sel.fd_max = ss.back()->serv_sock;
	// cerr << ss.back()->serv_sock;

	while (1){
		sel.selectoo();
		if (sel.ret == -1){
			perror("selectEr: ");
			cerr << "select went wrong.\n";
			break ;
		}
		if (sel.ret == 0) { // close remain sockets and data
			for (int i = 0; i < sel.fd_max + 1; i++){
				if (FD_ISSET(i, &sel.readfds) && i > ss.back()->serv_sock){	
					FD_CLR(i, &sel.readfds);
					close(i);
				}
			}
			FD_ZERO(&sel.writefds);
			clnt_serv.clear();
			req.clearReq();
			res.clearRes();
			// dprintf(2, "timeout\n");
			continue ;
		}
		// cout << "{{" << fd_num << "}}";
		for (int i = 0; i < sel.fd_max + 1; i++) {
			// cerr << fd_max << "|||||||||" << '\n';
			if(FD_ISSET(i, &sel.cpy_wfds)){
				handle_response(i);
			}
			if (FD_ISSET(i, &sel.cpy_rfds)){
				if (i <= ss.back()->serv_sock){ // connection request;
					adr_sz = sizeof(clnt_adr);
					clnt_sock = ft_accept(i, (struct sockaddr *) &clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &sel.readfds);
					if (sel.fd_max < clnt_sock){
						sel.fd_max = clnt_sock;
					}
					clnt_serv.insert(pair<int, int> (clnt_sock, i));
					// cerr << "|||||" << clnt_sock << " :: " << i << "|||||" << '\n';
					printf("connected client: %d \n", clnt_sock);
				}
				else { // read message from client!
					if ((str_len = ft_recv(i, buf, buff_size - 1, "recv :")) == -1)
						; // read request add protection
					// dprintf(2, "|||||||||||||strlen = %d||||||", str_len);
					if (str_len <= 0){ // close request!
						FD_CLR(i, &sel.readfds);
						if(FD_ISSET(i, &sel.cpy_wfds))
							FD_CLR(i, &sel.writefds);
						close(i);
						req.eraseReq(i);
						res.eraseRes(i);
						clnt_serv.erase(i);
						printf("close client %d \n", i);
					}
					else {
						handle_request(buf, str_len, i, sel.fd_max);
					}
				}
			}
		}
	}
}