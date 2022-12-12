/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rezzahra <rezzahra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 21:37:00 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/11 01:52:13 by rezzahra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// bool fileExists(std::string &Url){
// 	ifstream f(Url.c_str());
// 	return f.good();
// }

std::string listDirectory(const char *path,std::string url, Request &rhs){
	std::string index;
	struct stat buffer;
	std::stringstream ss;
	std::string sizet;
	off_t size;
	index = "<!DOCTYPE html>\n";
	index.append("<html lang=\"en\">\n<head>\n");
	index.append("<meta charset=\"UTF-8\">\n<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Document</title>\n");
	index.append("</head>\n<body>\n");
	index.append("<h1>Index Of ");
	index.append(std::string(path));
	index.append("</h1>\n<ul>\n");
	DIR *dp = opendir(path);
	if (!dp)
		return index;
	struct dirent *dip;
	dip = readdir(dp);
	while(dip != NULL){
		std::string name = std::string(dip->d_name);
		std::string name1 = url + name;
		stat(dip->d_name,&buffer);
		size = buffer.st_size;
		ss << size;
		sizet = ss.str();
		ss.str("")     ;
		index.append("<li><a href=\"http://");
		index.append(rhs.getHeader("Host"));
		index.append(name1);
		index.append("\">");
		std::string completer = "</li>\n";
		completer = name + "</a><p>" + sizet + " bytes </p>"+ completer;
		index.append(completer);
		dip = readdir(dp);
	}
	index.append("</ul>\n</body>\n</html>");
	closedir(dp);
	return index;
}

bool directoryExists(std::string &Url){
	struct stat buffer;
	stat(Url.c_str(),&buffer);
	return (S_ISDIR(buffer.st_mode));
}

bool checkMethod(Request &rhs){
	std::map <std::string, std::string>::iterator it;
	std::string method;
	it = rhs.hd.hd.find("firstline");
	if (it != rhs.hd.hd.end())
		method = it->second.substr(0,it->second.find(' '));
	std::string arr[3] = {"GET" , "POST" , "DELETE"};
	for(int i = 0;i < 3;i++){
		if (!method.compare(arr[i]))
			return true;
	}
	return false;
}

bool UriTooLong(std::string lhs){
		return (lhs.size() > 2048);
}

bool checkURI(std::string lhs){
	std::string chars = "0123456789abcdefghIjklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_~+#,%&=*;:@";
	for (int i = 0;i < lhs.size(); i++){
		if (chars.find(lhs[i]) == string::npos)
			return false;
	}
	return true;
}

bool checkEntity(Request &rhs,Server& lhs){
	std::stringstream ss;
	uint64_t sizeFile;
	serverInfo in;
	
	std::map<std::string,std::string>::iterator it = rhs.hd.hd.find("Content-Length");
	std::map<std::string,std::string>::iterator that = rhs.hd.hd.find("Host");
	ss << it->second;
	ss >> sizeFile;
	in = lhs.info;
	return (sizeFile > in.clientBodySize);
}

void BadRequest1(Request rhs,Response &lhs){
	std::map <std::string, std::string>::iterator it;
	std::string method;
	it = rhs.hd.hd.find("firstline");
	if (it != rhs.hd.hd.end()){
		method = it->second.substr(0,it->second.find(' '));
		if (method == "POST" && rhs.hd.hd.find("Content-Length") == rhs.hd.hd.end() && rhs.hd.hd.find("Transfer-Encoding") == rhs.hd.hd.end()){
			lhs.Code.code = LengthRequired;
			lhs.Code.reason = "Length Required";
			lhs.Code.HTTPv = "HTTP/1.1";
			lhs.filled = true;
		}
	}
}

std::string getUrl(Request &rhs){
	std::map <std::string, std::string>::iterator it;
	std::string url = "";
	it = rhs.hd.hd.find("firstline");
	if (it != rhs.hd.hd.end()){
		stringstream s(it->second);
		std::getline(s,url,' ');
		std::getline(s,url,' ');
	}
	return url;
}

std::string getMethod(Request &rhs){
	std::map <std::string, std::string>::iterator it;
	std::string method = "";
	it = rhs.hd.hd.find("firstline");
	if (it != rhs.hd.hd.end()){
		stringstream s(it->second);
		std::getline(s,method,' ');
	}
	return method;
}

std::string getHost(Request &rhs){
	std::map<std::string,std::string>::iterator that = rhs.hd.hd.find("Host");
	std::cout << that->second.substr(0,that->second.find(':')) << std::endl;
	return (that->second.substr(0,that->second.find(':')));
}

void BadRequest2(Request rhs,Response &lhs){
	std::map <std::string, std::string>::iterator it;
	std::string url = getMethod(rhs);
	if (it != rhs.hd.hd.end()){
		if (!checkURI(url)){
			lhs.Code.code = BadRequest;
			lhs.Code.reason = "Bad Request";
			lhs.Code.HTTPv = "HTTP/1.1";
			generateErrorHtml(lhs);
			lhs.isBodyFile = false;
			lhs.filled = true;
		}
		if (UriTooLong(url)){
			lhs.Code.code = UriToLong;
			lhs.Code.reason = "URI Too Long";
			lhs.Code.HTTPv = "HTTP/1.1";
			generateErrorHtml(lhs);
			lhs.isBodyFile = false;
			lhs.filled = true;
		}
	}
}

void isRequestwellFormed(Response *rhs, Request request,Server& lhs){
	// Response *rhs = new Response;
	rhs->filled = false;
	std::map <std::string, std::string>::iterator it;
	it = request.hd.hd.find("Transfer-Encoding");
	if (it->second != std::string("chunked") && it != request.hd.hd.end()){
		rhs->Code.code = NotImplemented;
		rhs->Code.reason = "Not Implemented";
		rhs->Code.HTTPv = "HTTP/1.1";
		generateErrorHtml(*rhs);
		rhs->isBodyFile = false;
		rhs->filled = true;
		return ;
	}
	BadRequest1(request,*rhs);
	if (rhs->filled)
		return ;
	BadRequest2(request,*rhs);
	if (rhs->filled)
		return ;
	if (getMethod(request) == "POST" && checkEntity(request,lhs)){
		rhs->Code.code = PayloadTooLarge;
		rhs->Code.reason = "Request Entity Too Large";
		rhs->Code.HTTPv = "HTTP/1.1";
		generateErrorHtml(*rhs);
		rhs->isBodyFile = false;
		rhs->filled = true;
		return ;
	}
}

bool decreasingLength(std::string a, std::string b){
	return (a.size() > b.size());
}

std::string biggestMatch(std::string rhs,std::vector<locations> locs){
	std::vector<locations>::iterator it;
	std::vector<std::string> locsNames;
	std::vector<std::string>::iterator that;
	for (it = locs.begin();it != locs.end();it++)
		locsNames.push_back(it->locationUri);
	std::sort(locsNames.begin(),locsNames.end(),decreasingLength);
	for (that = locsNames.begin();that != locsNames.end();that++){
		// std::cerr << "|hi " << *that << " bye|";
		if((*that) == rhs.substr(0,(*that).size()))
			return (*that);
	}
	return (std::string(""));
}

bool matchLocUri(Request &rhs,Server& server){
	std::string url = getUrl(rhs);
	if (biggestMatch(url, server.info.locs) == std::string("")){
		std::cerr << "false\n";
		return (false);
	}
	std::cerr << "true\n";
	return (true);
}

locations &getLoc(std::string location,serverInfo &rhs){
	std::vector<locations>::iterator it;
	for (it = rhs.locs.begin();it < rhs.locs.end();it++){
		if(it->locationUri == location)
			break;
	}
	return (*it);
}

locations *getRedirection(Request &rhs,Server& server){
	locations &p = getLoc(biggestMatch(getUrl(rhs),server.info.locs),server.info);
	return (&p);
}

bool isMethodAllowed(Request& rhs,Server& lhs,locations* loc){
	serverInfo server;
	std::string method = getMethod(rhs);
	server = lhs.info;
	std::vector<std::string>::iterator it;
	for(it = loc->acceptedMeth.begin();it < loc->acceptedMeth.end();it++){
		if(method.compare(*it) == 0)
			return true;
	}
	return false;
}

void Response::display(void){
	std::cout << Code.code << std::endl;
	std::cout << Code.HTTPv << std::endl;
	std::cout << Code.reason << std::endl;
	std::cout << body << std::endl;
	std::cout << contentLength << std::endl;
	std::cout << contentType << std::endl;
}

void Responsehandler(Response *rhs, Request &request,Server& lhs){
	// Response *rhs;
	std::string url = getUrl(request);
	rhs->con = request.getHeader("Connection");
	isRequestwellFormed(rhs, request,lhs);
	if (rhs->filled)
		return ;
	rhs->uri.path = biggestMatch(url,lhs.info.locs);
	rhs->uri.query = (url.find("?") != string::npos) ? url.substr(url.find("?"),url.npos) : "";
	rhs->uri.resource = url.substr(url.find_last_of("/") ,url.find("?") - url.find_last_of("/"));
	if (!matchLocUri(request,lhs)){
		std::cerr << "frome 1" << "\n";
		rhs->Code.code = NotFound;
		rhs->Code.reason = "Not Found";
		rhs->Code.HTTPv = "HTTP/1.1";
		generateErrorHtml(*rhs);
		rhs->isBodyFile = false;
		return ;
	}
	if (getRedirection(request,lhs)->redirString.first != -1)
	{
		std::cerr << "frome 2" << "\n";
		rhs->Code.code = MovedPermanently;
		rhs->Code.reason = "Moved Permanently";
		rhs->Code.HTTPv = "HTTP/1.1";
		rhs->Location = getRedirection(request,lhs)->redirString.second;
		return ;
	}
	if (!isMethodAllowed(request,lhs,getRedirection(request,lhs))){
		std::cerr << "frome 3" << "\n";
		rhs->Code.code = MethodNotAllowed;
		rhs->Code.reason = "Method Not Allowed";
		generateErrorHtml(*rhs);
		rhs->isBodyFile = false;
		rhs->Code.HTTPv = "HTTP/1.1";
		return ;
	}
	else {
		startserving(request,lhs,rhs);
		// rhs->Code.code = Ok;
		// rhs->Code.reason = "Ok";
		// rhs->Code.HTTPv = "HTTP/1.1";
	}
}
