/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rezzahra <rezzahra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 23:20:53 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/06 12:06:45 by rezzahra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
static void tokenize(std::string str, std::string delim, std::pair<std::string,std::string> &rhs){
	if (str.find(delim) != string::npos){
		rhs.first = str.substr(0,str.find(delim));
		rhs.second = str.substr(str.find(delim) + delim.length(),str.length() - rhs.first.length() - delim.length());
	}
}

std::map<std::string,std::string> fillMap(void){
	std::map<std::string,std::string> mime;
	std::string delim,value,key,line,content = "./mime.txt";
	delim = "===";
	std::pair<std::string,std::string> rhs;
	ifstream file1;
	file1.open(content);
	while (!file1.eof() && getline(file1,line, '\n')){
		tokenize(line,delim,rhs);
		mime.insert(rhs);
	}
	return (mime);
}

int main(){
	std::string r = "index.html";
	std::map<std::string,std::string> mime = fillMap();
	std::cout << mime.at(r.substr(r.find("."),std::string::npos));
}