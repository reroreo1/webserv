#include "Request.hpp"
#define nameT "fileT"

RequestHandler::RequestHandler(){
	
}

void RequestHandler::handleReq(int clnt, char *buf, int bufSize){
	if (!keyInMap(reqpool, clnt))
		reqpool.insert(std::pair<int, Request>(clnt, Request()));
	reqpool.at(clnt).add(buf, bufSize);
}

void RequestHandler::refReq(int clnt){
	eraseReq(clnt);
	reqpool.insert(std::pair<int, Request>(clnt, Request()));
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
Request::Request(): fase(HEADER), tmpBufLen(0), newChunk(true) ,chunkSize(0){
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
	int tes;
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
			std::cerr << "{s{ " << chuncklen << "}}"; // debug
			chunkSize = hexToi(chuncklen.c_str());
			std::cerr << "\n\nchunk size = " << chunkSize << "\n\n";// getting the right chunk size
			if (!chunkSize){
				fase = FINISHED;
				close(tmpfd.first);
				tmpfd.second = false;
				std::cerr << "{{ new chunk" << chunkSize << "}}"; // debug
				tmpBufLen = 0;
				return ;
			}
			tmpBufLen -=  (pos - tmpbuf) + 2;// threat buffer
			memmove(tmpbuf, pos + 2, tmpBufLen);
		}
		if (chunkSize <= BUFSIZ && (pos = lookFor(tmpbuf, "\r\n", 2, tmpBufLen))){
			ft_writeFds(tmpfd.first, tmpbuf, chunkSize, "error : writing chuncked");
			newChunk = true;
			tmpBufLen -= chunkSize + 2;
			memmove(tmpbuf, tmpbuf + chunkSize + 2, tmpBufLen);
			chunkSize = 0;
			// std::cerr << "-----------"<< chunkSize <<"---------------";
			continue ;
		}
		if (chunkSize <= BUFSIZ){
			ft_writeFds(tmpfd.first, tmpbuf, tmpBufLen - 3, "error : writing chuncked");
			chunkSize -= (tmpBufLen - 3);
			memmove(tmpbuf, tmpbuf + tmpBufLen - 3, 3);
			tmpBufLen = 3;
		}
		else {
			ft_writeFds(tmpfd.first, tmpbuf, tmpBufLen, "error : writing chuncked");
			chunkSize -= (tmpBufLen);
			tmpBufLen = 0;
		}
		// std::cerr << "{{ " << chunkSize << "}}"; // debug
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
	memmove(tmpbuf + tmpBufLen, buf, bufSize);
	// tmpBufLen = 0;
	tmpBufLen += bufSize;
	bufSize = 0;
	ft_writeFds(tmpfd.first, tmpbuf, tmpBufLen, "error : writing chuncked");
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

static string getUrl(std::string fl){
	std::string::iterator it1 = find(fl.begin(), fl.end(), ' ');
	std::string::iterator it2 = find(it1 + 1, fl.end(), ' ');
	return (std::string(it1, it2));
}

int Request::checkingEn(int bufSize){
	if (tmpBufLen + bufSize >= 2048){
		fase = ETL;
		return 1;
	}
	if (getHeader("firstline") != "NF"){
		std::string url = getUrl(getHeader("firstline"));
		std::cerr << "from checking En" << url << std::endl; 
		if (url.length() >= 2048){
			fase = ETL;
			return 1;
		}
	}
	return 0;
}

void Request::headerFase(char *&buf, int &bufSize){
	char *pos;
	int extra;
	char headerBuff[BUFSIZE * 3];

	memcpy(headerBuff, tmpbuf, tmpBufLen);
	if ((pos = lookFor(buf, "\r\n\r\n", 4, bufSize))){
		memcpy(&headerBuff[tmpBufLen], buf, pos - buf + 2);
		parseheader(headerBuff, pos - buf + 2 + tmpBufLen);
		if (checkHeader() || checkingEn(bufSize))
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
	// checking fields length;
	if (pos){
		tmpBufLen = bufSize - (pos - buf + extra);
		bufSize = 0;
		memmove(tmpbuf, pos + extra, tmpBufLen);
		printMap(hd.hd);
		return ;
	}
	if (checkingEn(bufSize))
		return ;
	tmpBufLen += bufSize;
	memmove(tmpbuf, headerBuff, tmpBufLen);
}

void Request::add(char *buf, int bufSize){
	char *pos;

	if (fase == HEADER)
		headerFase(buf, bufSize);
	if (fase == BODY)
		bodyFase(buf, bufSize);
	if (fase == ETL)
		std::cerr << "||ETL||";
}