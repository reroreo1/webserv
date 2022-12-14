#include "Response.hpp"

// -----------responseHandler----------------------
Response::Response(int cl, std::map<std::string, std::string> &m): fd(-1), stat(HEADER), bufin(0), byteFromBody(0), remSize(0), mime(m){
	clnt = cl;
	isBodyFile = true;
	Location = "";
	contentType = "";
	contentLength = "";
	body = "";
}

void Response::fileSender(){
	if (stat == HEADER){
		if (isBodyFile){
			fd = ft_open(body.c_str(), O_RDONLY);
			byteFromBody = fd_size(fd);
		} 
		else {
			byteFromBody = body.length();
		}
		makeHeader(*this);
		bWrite(resBuf, bufin);
		stat = BODY;
	}
	if (stat != BODY)
		return ;
	int ret;
	if (bufin < BUFSIZE){
		if (isBodyFile){
			ret = ft_read(fd, resBuf + bufin, BUFSIZE - bufin, "from fileSender");
			bufin += ret;
			byteFromBody -= ret;
		}
		else{
			ret = BUFSIZE - bufin < body.length() ? BUFSIZE - bufin : body.length();
			memmove(resBuf + bufin, (char *)body.c_str(), ret);
			body = body.substr(ret, body.length());
			bufin += ret;
			std::cerr << "\nret=" << ret << std::endl;
			stat = FINISHED;
			ft_send(clnt, resBuf, bufin, "filesSender.");
			bufin = 0;
		}
	}
	if(!byteFromBody){
		stat = FINISHED;
		// std::cerr << "\nbufin=" << bufin << std::endl;
		if (isBodyFile)
			close(fd);
		// stat = FINISHED;
	}
	ft_send(clnt, resBuf, bufin, "filesSender.");
	bufin = 0;
}

ResponseHandler::ResponseHandler(){
	mime = fillMap();
};

ResponseHandler::~ResponseHandler(){
	
};

Response &ResponseHandler::handleRes(int clnt, Request &req, Server &ser){
	if (!keyInMap(respool, clnt))
		respool.insert(std::pair<int, Response>(clnt, Response(clnt, mime)));
	Responsehandler(&respool.at(clnt), req, ser);
	return respool.at(clnt);
};

Response &ResponseHandler::getResponse(int clnt){
	// if (!keyInMap(respool, clnt))
		return respool.at(clnt);
};


void ResponseHandler::eraseRes(int clnt){
	if (keyInMap(respool, clnt))
		respool.erase(clnt);
};

void ResponseHandler::clearRes(){
	respool.clear();
};