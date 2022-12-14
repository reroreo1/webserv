#ifndef WORKER_HPP
#define WORKER_HPP
#include "globalheader.hpp"
#include "Response.hpp"
using namespace std;


// struct RequestHandler;
struct selectInfo{
	int fd_max;
	int ret;
	fd_set readfds;
	fd_set cpy_rfds;
	fd_set writefds;
	fd_set cpy_wfds;
	struct timeval timeout;

	selectInfo();
	void selectoo();
	int acceptoo(){return 1;};
	~selectInfo();
};

struct Worker{
	RequestHandler req;
	ResponseHandler res;
	// std::vector<int > clntToRead;
	// std::vector<int > clntToWrite;
	int str_len;
	selectInfo sel;
	map<int ,int> clnt_serv;
	size_t buff_size;
	vector<Server *> ss;

	Worker();

	Worker(vector<Server *> &serv_sock, size_t bs);

	int handle_request(char *buff, int str_len, int clnt,
	 int &fd_max);

	int handle_response(int clnt);

	// void setClntWr(int fd);
	// void setClntWr(std::vector<int>::iterator clnt);
	// void setClntRe(int fd);
	// void setClntRe(std::vector<int>::iterator clnt);
	void cleaning();
	
	// void clrClntWr(int fd);
	// void clrClntWr(std::vector<int>::iterator clnt);
	// void clrClntRe(int fd);
	// void clrClntRe(std::vector<int>::iterator clnt);
	
	void updatingThing();
	void start();
};
#endif