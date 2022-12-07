/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rezzahra <rezzahra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 23:20:53 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/07 22:28:51 by rezzahra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
std::string unts(std::uint16_t n){
	std::stringstream ss;
	ss << n;
	return (ss.str());
}

std::string& makeHeader(Response &rhs,Request &lhs){
	std::string header;
	std::map<std::string,std::string>::iterator it = lhs.hd.hd.find("Connection");
	header = rhs.Code.HTTPv + " " + unts(rhs.Code.code) + " " + rhs.Code.reason + "\n";
	header += it->first + ": " + it->second;
	if (getMethod(lhs) == "GET"){
		header += "Content: " + rhs.contentLength;
} 

int main(){
	std::string p = "rachid";
	std::string a = "daddy";
	std::cout << p + " " + a << std::endl;
}