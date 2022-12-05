#include "Request.hpp"
#define nameT "fileT"

// Request handler
RequestHandler::RequestHandler(){
	
}

void RequestHandler::handleReq(int clnt, char *buf, int bufSize, fd_set *w){
	if (!keyInMap(reqpool, clnt))
		reqpool.insert(std::pair<int, Request>(clnt, Request(w)));
	reqpool.at(clnt).add(buf, bufSize);
}

void RequestHandler::eraseReq(int clnt){
	if (keyInMap(reqpool, clnt))
		reqpool.erase(clnt);
}

Request &RequestHandler::getRequest(int clnt){
	return reqpool.at(clnt);
}

void RequestHandler::clearReq(){
	reqpool.clear();
}
//request header
Rheader::Rheader(): firstLine(true){};

void Rheader::addToHeader(char *b, char *e){
	if (firstLine){
		hd.insert(std::pair<std::string, std::string>(std::string("firstline"), std::string(b, e)));
		firstLine = false;
		return ;
	}
	char *sep = lookFor(b, ":", 1, e - b);
	if (sep)
		hd.insert(std::pair<std::string, std::string>(std::string(b, sep), std::string(sep + 2, e)));
}

//request
Request::Request(fd_set *w): fase(HEADER), tmpBufLen(0), newChunk(true) ,chunkSize(0){
	writefds = w;
	tmpfd.first = ftOpen(std::string(nameT));
	tmpfd.second = true;
}

Request::~Request(){
	// if(tmpfd.second)
	// 	close(tmpfd.first);
}


void Request::parseheader(char *buf, int size){
	char *lastPos = buf;

	for (char *curPos = lookFor(lastPos, "\r\n", 2, size); curPos; curPos = lookFor(lastPos, "\r\n", 2, size)){
		hd.addToHeader(lastPos, curPos);
		size -= curPos - lastPos + 2;
		lastPos = curPos + 2;
	}
}

void Request::hundleChunkedBody(char *&buf, int &size){
	char *pos;
	
	// tmpfd.first = ftOpen(std::string(nameT));
	memmove(tmpbuf + tmpBufLen, buf, size);
	tmpBufLen += size;
	if (fase == FINISHED)
		return ;
	while (tmpBufLen > 3){
		if (newChunk){
			if (!(pos = lookFor(tmpbuf, "\r\n", 2, tmpBufLen))){
				std::cerr << "something want wrongb\n";
				return ;
			};
			std::string chuncklen(tmpbuf, pos);
			chunkSize = hexToi(chuncklen.c_str());
			std::cerr << "{s{ " << chuncklen << "}}"; // debug
			std::cerr << "\n\nchunk size = " << chunkSize << "\n\n";// getting the right chunk size
			if (!chunkSize){
				fase = FINISHED;
				close(tmpfd.first);
				tmpfd.second = false;
				std::cerr << "{{ " << chunkSize << "}}"; // debug
				tmpBufLen = 0;
				return ;
			}
			tmpBufLen -=  (pos - tmpbuf) + 2;// threat buffer
			memmove(tmpbuf, pos + 2, tmpBufLen);
		}
		if (chunkSize <= BUFSIZE && (pos = lookFor(tmpbuf, "\r\n", 2, tmpBufLen))){
			ft_writeFds(tmpfd.first, tmpbuf, pos - tmpbuf, "error : writing chuncked", writefds);
			chunkSize -= (pos - tmpbuf);
			newChunk = true;
			tmpBufLen -= (pos - tmpbuf) + 2;
			std::cerr << "\n\n";
			bWrite(tmpbuf, (pos - tmpbuf) + 2);
			memmove(tmpbuf, pos + 2, tmpBufLen);
			std::cerr << "--------------------------";
			bWrite(tmpbuf, tmpBufLen);
			continue ;
		}
		ft_writeFds(tmpfd.first, tmpbuf, tmpBufLen - 3, "error : writing chuncked", writefds);
		chunkSize -= (tmpBufLen - 3);
		memmove(tmpbuf, tmpbuf + tmpBufLen - 3, 3);
		tmpBufLen = 3;
		std::cerr << "{{ " << chunkSize << "}}"; // debug
		newChunk = false;
		break ;
	}
}

void Request::bodyFase(char *&buf, int &bufSize){
	static ssize_t bodyLen;
	char *pos;

	if (getHeader("Transfer-Encoding") == "chunked"){
		hundleChunkedBody(buf, bufSize);
		return ;
	}
	// tmpfd.first = ftOpen(std::string(nameT));
	memmove(tmpbuf + tmpBufLen, buf, bufSize);
	// tmpBufLen = 0;
	tmpBufLen += bufSize;
	bufSize = 0;
	ft_writeFds(tmpfd.first, tmpbuf, tmpBufLen, "error : writing chuncked", writefds);
	bodyLen += tmpBufLen;// check content len
	tmpBufLen = 0;
	if (bodyLen == atoi(getHeader("Content-Length").c_str())){
		fase = FINISHED;
		close(tmpfd.first);
		tmpfd.second = false;
		std::cerr << "request handling finished.\n";
	}
}

std::string Request::getHeader(std::string name) {
	if (keyInMap(hd.hd, name))
		return hd.hd.at(name);
	return std::string("NF");
}

int	Request::checkHeader(){
	if (keyInMap(hd.hd, string("firstline"))){
		fase = strncmp(hd.hd.at("firstline").c_str(), "POST", 4) ? FINISHED : BODY;
		if (fase == FINISHED)
			close(tmpfd.first);
			tmpfd.second = false;
	}
	return (0);
}

void Request::headerFase(char *&buf, int &bufSize){
	char *pos;
	int extra;
	char headerBuff[BUFSIZE * 2];

	memcpy(headerBuff, tmpbuf, tmpBufLen);
	if ((pos = lookFor(buf, "\r\n\r\n", 4, bufSize))){
		memcpy(&headerBuff[tmpBufLen], buf, pos - buf + 2);
		parseheader(headerBuff, pos - buf + 2 + tmpBufLen);
		if (checkHeader())
			return ;
		extra = 4;
	} 
	else if ((pos = lookForLast(buf, "\r\n", 2, bufSize))){
		memcpy(&headerBuff[tmpBufLen], buf, pos - buf + 2);
		parseheader(headerBuff, pos - buf + 2 + tmpBufLen);
		extra = 2;
	}
	else {

	}
	tmpBufLen = bufSize - (pos - buf + extra);
	bufSize = 0;
	memmove(tmpbuf, pos + extra, tmpBufLen);
	printMap(hd.hd);
}

void Request::add(char *buf, int bufSize){
	char *pos;

	if (fase == HEADER)
		headerFase(buf, bufSize);
	if (fase == BODY)
		bodyFase(buf, bufSize);
}