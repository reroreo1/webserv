#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "globalheader.hpp"
#include "Request.hpp"

// #include <sys/stat.h>
// #include <sstream>
// #include <dirent.h>
// #include <sys/types.h>
//characters allowed in url
//0 1 2 3 4 5 6 7 8 9
//a b c d e f g h I j k l m n o p q r s t u v w x y z
//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
//.-_~+#,%&=*;:@
	enum
	{
		Continue = 100,
		Ok = 200,
		Created = 201,
		NoContent = 204,
		MovedPermanently = 301,
		BadRequest = 400,
		Forbidden = 403,
		NotFound = 404,
		MethodNotAllowed = 405,
		Conflict = 409,
		LengthRequired = 411,
		PayloadTooLarge = 413,
		UriToLong = 414,
		UnsupportedMediaType = 415,
		InternalServerError = 500,
		NotImplemented = 501,
		HttpVersionNotSupported = 505
	};

	struct URI{
		// /path/to/index.html?usermame
		std::string path;
		std::string query;
		std::string resource;

		inline void displayUri(){
			dprintf(2, "|%s|, |%s|, |%s|", path.c_str(), query.c_str() , resource.c_str());
		};
	};

	struct status {
		std::uint16_t code;
		std::string reason;
		std::string HTTPv;
	};

	struct Response{
		std::map<std::string, std::string> &mime;
		int clnt;
		bool filled;
		bool isBodyFile;
		// std::map<std::string,std::string> &mime;
		URI uri;
		status Code;
		std::string Location;
		std::string contentType;
		std::string contentLength;
		std::string body;
		std::string con;
		void display();
		//-----added----
		enum {HEADER, BODY, FINISHED} stat;
		char resBuf[BUFSIZ];
		int bufin;
		int fd;
		size_t fileSizeRemain;
		int byteFromBody;
		char remain[BUFSIZ];
		int remSize;
		Response (int, std::map<std::string, std::string> &m);
		void fileSender ();
	};
/*---------------------GETTERS---------------------------------*/
// need function to fill buff with header when it's needed

/*---------------------GETTERS---------------------------------*/
	locations* getRedirection(Request &rhs,Server& server);
	locations& getLoc(std::string location,serverInfo &rhs);
	std::string getUrl(Request &rhs);
	std::string getMethod(Request &rhs);
	std::string getHost(Request &rhs);
/*---------------------CHECKS----------------------------------*/
	// bool fileExists(std::string &Url);
	bool directoryExists(std::string &Url);
	bool checkMethod(Request &rhs);
	bool UriTooLong(std::string lhs);
	bool checkURI(std::string lhs);
	bool checkEntity(Request &rhs,Server& lhs);
	void BadRequest1(Request rhs,Response &lhs);
	void BadRequest2(Request rhs,Response &lhs);                   
	void isRequestwellFormed(Response *rhs, Request request,Server& lhs);
	bool decreasingLength(std::string a, std::string b);
	std::string biggestMatch(std::string rhs,std::vector<locations> locs);
	bool matchLocUri(Request &rhs,Server& server);
	bool isMethodAllowed(Request& rhs,Server& lhs,locations& loc);
	void Responsehandler(Response *rhs, Request &request,Server& lhs);
	std::map<std::string,std::string> fillMap(void);
	std::string listDirectory(const char *path,std::string url,Request &rhs);
	void startserving(Request &rhs,Server& server,Response* pons);
	std::string makeHeader(Response &rhs);
	void generateErrorHtml(Response &rhs);
	struct ResponseHandler{
		std::map<std::string, std::string> mime;
		std::map<int, Response> respool;
		// std::vector<Server *> &s;
		ResponseHandler();
		~ResponseHandler();
		Response &handleRes(int clnt, Request &req, Server &ser);
		Response &getResponse(int clnt);
		void eraseRes(int cnlt);
		void clearRes();

	};

#endif