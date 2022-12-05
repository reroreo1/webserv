#include "Response.hpp"

std::string read(std::string& file) {
    std::ifstream in;
    std::string text="";
    string s;
    in.open(file, ios::in);
    while (std::getline(in, s)) {
        text.append(s);
        text.append("\n");
    }
    return text; 
}
std::string getIndex(std::string file,std::vector<std::string> index){

}
Response* startserving(Request &rhs,Server& server,Response *lhs,Response* pons){
	serverInfo ser = server.info;
	locations p = getLoc(lhs->uri.path,ser);
	struct stat buff;	
	std::string res = p.root.append(lhs->uri.resource);
	std::string lol = "." + res;
	std::string::iterator it = lol.end();
	it--;
	if (getMethod(rhs) == "GET"){
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
			if (getLoc(pons->uri.path,server.info).index.size() == 0 && getLoc(pons->uri.path,server.info).autoIndex == 0){
				pons->Code.code = Forbidden;
				pons->Code.reason = "Forbidden";
				pons->Code.HTTPv = "HTTP/1.1";
				return (pons);
			}
			else if (getLoc(pons->uri.path,server.info).index.size() != 0)
			{
				pons->Code.code = Ok;
				pons->Code.reason = "Ok";
				pons->Code.HTTPv = "HTTP/1.1";
				pons->body = getLoc(pons->uri.path,server.info).root + pons->uri.resource;
				stat(pons->body.c_str(),&buff);
				pons->contentLength = buff.st_size;
				pons->contentType = 
			}
		}
		else if (fileExists(lol)){
			
		}
	}
}
