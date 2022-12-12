#include "ResponseConstructor.hpp"

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

ResponseCon::ResponseCon(Response &res){
	if (res.stat == 2)
		return ;
	if (res.stat == 1){
		// sent body in get ...;
		return ;
	}
	constructHeader(res);
}

void ResponseCon::constructHeader(Response &res){
	firstLine(res);
	
}

void ResponseCon::tPrint(Response &res, char *str, size_t size){
	int rem = BUFSIZ - res.bufin >= size ? -1 : BUFSIZ - res.bufin;
	memmove(res.resBuf + res.bufin, str, rem == -1 ? size : rem);
	res.bufin += rem == -1 ? size : rem;
	if (rem == -1 && (res.bufin != BUFSIZ)){
		std::cerr << "something is not going as accepected.";
		exit(1);
	}
	if (rem != -1){
		memmove(res.remain + res.remSize, str + rem, size - rem);
		res.remSize += size - rem;
	}
}

void ResponseCon::addDel1(Response &res){ tPrint(res, "\r\n", 2);}

void ResponseCon::addDel2(Response &res){ tPrint(res, "\r\n\r\n", 4);}

void ResponseCon::firstLine(Response &res){
	tPrint(res, "HTTP/1.1 ", 9);
	std::stringstream st;
	st << res.Code.code;
	tPrint(res, (char *) st.str().c_str(), st.str().length());
	tPrint(res, (char *) " ", 1);
	tPrint(res, (char *) res.Code.reason.c_str(), res.Code.reason.length());
	addDel1(res);
}