#include "Response.hpp"

// ('SERVER_SOFTWARE',   'The server software is: ',
//          'SERVER_NAME',       'The server hostname, DNS alias, or IP address is: ',
//          'GATEWAY_INTERFACE', 'The CGI specification revision is: ',   
//          'SERVER_PROTOCOL',   'The name and revision of info protocol is: ',
//          'SERVER_PORT',       'The port number for the server is: ',
//          'REQUEST_METHOD',    'The info request method is: ',
//          'PATH_INFO',         'The extra path info is: ',
//          'PATH_TRANSLATED',   'The translated PATH_INFO is: ',
//          'DOCUMENT_ROOT',     'The server document root directory is: ',
//          'SCRIPT_NAME',       'The script name is: ',
//          'QUERY_STRING',      'The query string is (FORM GET): ',
//          'REMOTE_HOST',       'The hostname making the request is: ',
//          'REMOTE_ADDR',       'The IP address of the remote host is: ',
//          'AUTH_TYPE',         'The authentication method is: ',
//          'REMOTE_USER',       'The authenticated user is: ',
//          'REMOTE_IDENT',      'The remote user is (RFC 931): ',
//          'CONTENT_TYPE',      'The content type of the data is (POST, PUT): ',
//          'CONTENT_LENGTH',    'The length of the content is: ',
//          'HTTP_ACCEPT',       'The MIME types that the client will accept are: ',
//          'HTTP_USER_AGENT',   'The browser of the client is: ',
//          'HTTP_REFERER',      'The URL of the referer is: ');

void headerToEnv(std::pair<std::string,std::string> header){
	char a[10000] =  {0};
	for (int i = 0;i < header.first.length();i++){
		a[i] = (char)toupper(header.first[i]);
		if (header.first[i] == '-')
			a[i] = '_';
	}
	setenv(a,header.second.c_str(),true);
}

void setCgiEnvir(Response& rhs,Request& lhs){
	setenv("SERVER_NAME","webserv/1.1",true);
	setenv("GATEWAY_INTERFACE","CGI/1.1",true);
	setenv("SERVER_PROTOCOL","HTTP/1.1",true);
	setenv("REQUEST_METHOD",getMethod(lhs).c_str(),true);
	setenv("QUERY_STRING",rhs.uri.query.c_str(),true);
	setenv("SCRIPT_NAME",,);
	setenv("DOCUMENT_ROOT",,);
	setenv("PATH_INFO",,);
	setenv("FCGI_ROLE","RESPONDER",true);
}