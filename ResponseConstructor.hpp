#ifndef RESPONSECONSTRUTOR_HPP
#define RESPONSECONSTRUTOR_HPP
#include "globalheader.hpp"
#include "Response.hpp"

struct ResponseCon{
	int st;
	ResponseCon(Response &res);
	void constructHeader(Response &res);
	void tPrint(Response &res, char *str, size_t size);
	void addDel1(Response &res);
	void addDel2(Response &res);
	void firstLine(Response &res);
};

#endif