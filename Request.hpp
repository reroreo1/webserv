#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "globalheader.hpp"
#include "server.hpp"
// #include "Response.hpp"

struct Rheader {
	std::map<std::string, std::string> hd;
	bool firstLine;

	Rheader();
	void addToHeader(char *begin, char *end);
};

struct Request {
	enum		{HEADER, BODY, FINISHED, ETL};
	Rheader		hd;
	std::pair<int, bool> tmpfd;
	bool		newChunk;
	ssize_t		chunkSize;
	char		tmpbuf[BUFSIZE * 3];
	int			tmpBufLen;
	int			fase;
	ssize_t		contLengh;

	Request();
	~Request();
	std::string getHeader(std::string);
	void add(char *buf, int bufSize);
	void headerFase(char *&, int &);
	void bodyFase(char *&, int&);
	void parseheader(char *buf, int size);
	// void setSets(fd_set *r, fd_set *w);
	int	 checkHeader();
	int checkingEn(int);
	void hundleChunkedBody(char *&, int&);
	void process();
};

struct RequestHandler{
	std::map<int, Request> reqpool;
	// std::vector<Server *> &s;

	RequestHandler();
	void handleReq(int clnt, char *buf, int bufSize);
	Request &getRequest(int clnt);
	void refReq(int);
	void eraseReq(int cnlt);
	void clearReq();
};

#endif