#include "Response.hpp"

static std::string itoss(size_t size){
	std::stringstream ss;
	ss << size;
	return (ss.str());
}

void generateErrorHtml(Response &rhs){
	std::string html;
	html = "<!DOCTYPE html>\n";
	html.append("<html lang=\"en\">\n<head>\n");
	html.append("<meta charset=\"UTF-8\">\n<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Document</title>\n");
	html.append("</head>\n<body>\n");
	html.append("<h1>");
	html.append(itoss(rhs.Code.code) + " " + rhs.Code.reason);
	html.append("</h1>\n");
	html.append("</body>\n</html>");
	rhs.contentType = "text/html";
	rhs.contentLength = itoss(html.length());
	rhs.body = html;
}

static std::string unts(std::uint16_t n){
	std::stringstream ss;
	ss << n;
	return (ss.str());
}

void addTHea(std::string &h, std::string toAdd, std::string check, bool del){
	if (check != ""){
		h += toAdd + check;
		if (del)
			h += "\n"; 
	}
}

std::string makeHeader(Response &rhs){
	std::string header;
	addTHea(header, rhs.Code.HTTPv + " " + unts(rhs.Code.code) + " ",  rhs.Code.reason, true);
	addTHea(header, "Content-Length: ",  rhs.contentLength, true);
	addTHea(header, "Content-Type: ",  rhs.contentType, true);
	addTHea(header, "Location: ",  rhs.Location, true);
	addTHea(header, "Connection", ": " + rhs.con, false);
	header += "\r\n\r\n";
	return (header);
} 
bool	checkFileLoc(locations loc){
	std::string check;
	std::vector<std::string>::iterator index;
	for (index = loc.index.begin(); index != loc.index.end(); index++){
		check = "." + loc.root + "/" + *index;
		if (fileExists(check)){
			return true;
		}
	}
	return false;
}
// location without a backslash
void startserving(Request &rhs,Server& server,Response* pons){
	serverInfo ser = server.info;
	locations p = getLoc(pons->uri.path,ser);
	struct stat buff;	
	std::string res = p.root.append(pons->uri.resource);
	std::string lol = "." + res;
	std::string::iterator it = lol.end();
	it--;
	if (getMethod(rhs) == "GET"){
		locations loc = getLoc(pons->uri.path,server.info);
		if (!fileExists(lol) && !directoryExists(lol)){
			pons->Code.code = NotFound;
			pons->Code.reason = "Not Found";
			pons->Code.HTTPv = "HTTP/1.1";
			generateErrorHtml(*pons);
			pons->isBodyFile = false;
			return ;
		}
		if (directoryExists(lol) && *it != '/')
		{
			pons->Code.code = MovedPermanently;
			pons->Code.reason = "Moved Permanently";
			pons->Code.HTTPv = "HTTP/1.1";
			pons->Location = getUrl(rhs) + '/';
			pons->isBodyFile = false;
			return ;
		}
		else if (directoryExists(lol) && *it == '/')
		{
			if (loc.index.size() == 0 && loc.autoIndex == 0){
				pons->Code.code = Forbidden;
				pons->Code.reason = "Forbidden";
				pons->Code.HTTPv = "HTTP/1.1";
				generateErrorHtml(*pons);
				pons->isBodyFile = false;
				return ;
			}
			else if (loc.index.size() != 0 && loc.autoIndex == 0)
			{
				if (checkFileLoc(loc)){
					if (stat(pons->body.c_str(),&buff) == -1)
						std::cerr << "say something.";
						pons->Code.code = Ok;
					pons->Code.reason = "OK";
					pons->Code.HTTPv = "HTTP/1.1";
					pons->contentLength = itoss(buff.st_size);
					pons->contentType = pons->mime.at(res.substr(res.find("."),std::string::npos));
				}
				else{	
					pons->Code.code = Forbidden;
					pons->Code.reason = "Forbidden";
					pons->Code.HTTPv = "HTTP/1.1";
					generateErrorHtml(*pons);
				}
				return ;
			}
			else if (loc.autoIndex){
				pons->Code.code = Ok;
				pons->Code.reason = "OK";
				pons->Code.HTTPv = "HTTP/1.1";
				pons->body = listDirectory(("." + loc.root).c_str(),loc.locationUri,rhs);
				pons->isBodyFile = false;
				pons->contentLength = itoss(pons->body.size());
				pons->contentType = pons->mime.at(".html");
				return ;
			}
		}
		else if (fileExists(lol)){
			// std::cerr << "from here";
			pons->Code.code = Ok;
			pons->Code.reason = "OK";
			pons->Code.HTTPv = "HTTP/1.1";
			pons->body = lol;
			if (stat(pons->body.c_str(),&buff) == -1)
				std::cerr << "say something.";
			pons->contentLength = itoss(buff.st_size);
			pons->contentType = pons->mime.at(res.substr(res.find("."),std::string::npos));
			return ;
		}
		//still needs cgi
	}
	else if (getMethod(rhs) == "POST"){
		std::vector<std::string>::iterator met = std::find(p.acceptedMeth.begin(),p.acceptedMeth.end(),"POST");
		if (met == p.acceptedMeth.end()){
			pons->Code.code = NotFound;
			pons->Code.reason = "Not Found";
			generateErrorHtml(*pons);
			pons->isBodyFile = false;
			pons->Code.HTTPv = "HTTP/1.1";
			return ;
		}
		else {
			pons->Code.code = Created;
			pons->Code.reason = "Created";
			pons->Code.HTTPv = "HTTP/1.1";
		}
		//create the file in the normal post and the transfer encoding chucked and ? CGI ?
	}
	else if (getMethod(rhs) == "DELETE"){
		deleteMethod(*pons,lol);
	}
}
