#ifndef WORKER_HPP
#define WORKER_HPP
#include "globalheader.hpp"
#include "confFile.hpp"
#include "Request.hpp"
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
	int str_len;
	selectInfo sel;
	map<int ,int> clnt_serv;
	size_t buff_size;
	vector<Server *> ss;

	Worker();

	Worker(vector<Server *> &serv_sock, size_t bs);

	int handle_request(char *buff, int str_len, int i, int &fd_max);

	int handle_response(int i);

	void start();
};
#endif