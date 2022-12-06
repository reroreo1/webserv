#include "Response.hpp"


Response* startserving(Request &rhs,Server& server,Response* pons){
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
			return (pons);
		}
		if (directoryExists(lol) && *it != '/')
		{
			pons->Code.code = MovedPermanently;
			pons->Code.reason = "Moved Permanently";
			pons->Code.HTTPv = "HTTP/1.1";
			pons->Location = getUrl(rhs) + '/';
			return (pons);
		}
		else if (directoryExists(lol) && *it == '/')
		{
			if (loc.index.size() == 0 && loc.autoIndex == 0){
				pons->Code.code = Forbidden;
				pons->Code.reason = "Forbidden";
				pons->Code.HTTPv = "HTTP/1.1";
				return (pons);
			}
			else if (loc.index.size() != 0)
			{
				std::vector<std::string>::iterator index = std::find(loc.index.begin(),loc.index.end(),"index.html");
				if (index != loc.index.end()){
					pons->body = loc.root + "index.html";
					pons->contentType = pons->mime.at(".html");
				}
				else{
					pons->body = loc.root + loc.index[0];
					pons->contentType = pons->mime.at(index[0].substr(index[0].find("."),std::string::npos));
				}
				pons->Code.code = Ok;
				pons->Code.reason = "Ok";
				pons->Code.HTTPv = "HTTP/1.1";
				stat(pons->body.c_str(),&buff);
				pons->contentLength = buff.st_size;
				return (pons);
			}
			else if (loc.autoIndex){
				pons->Code.code = Ok;
				pons->Code.reason = "Ok";
				pons->Code.HTTPv = "HTTP/1.1";
				pons->body = listDirectory(loc.root.c_str(),loc.locationUri);
				pons->contentLength = pons->body.size();
				pons->contentType = pons->mime.at(".html");
				return (pons);
			}
		}
		else if (fileExists(lol)){
			pons->Code.code = Ok;
			pons->Code.reason = "Ok";
			pons->Code.HTTPv = "HTTP/1.1";
			pons->body = lol;
			stat(pons->body.c_str(),&buff);
			pons->contentLength = buff.st_size;
			pons->contentType = pons->mime.at(res.substr(res.find("."),std::string::npos));
			return (pons);
		}
		//still needs cgi
	}
	else if (getMethod(rhs) == "POST"){
		std::vector<std::string>::iterator met = std::find(p.acceptedMeth.begin(),p.acceptedMeth.end(),"POST");
		if (met == p.acceptedMeth.end()){
			pons->Code.code = NotFound;
			pons->Code.reason = "Not Found";
			pons->Code.HTTPv = "HTTP/1.1";
			return (pons);
		}
		//create the file in the normal post and the transfer encoding chucked and ? CGI ?
	}
	else if (getMethod(rhs) == "DELETE"){
		
	}
}
