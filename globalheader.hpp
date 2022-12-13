#ifndef GLOBALHEADER_HPP
#define GLOBALHEADER_HPP

#define BUFSIZE 1024

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <map>
#include <fcntl.h>
#include <errno.h> // to be removed
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <exception>
#include <algorithm>
#include <signal.h>
#include <sys/stat.h>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <ftw.h>
// #include "SocketS.hpp"
// #include "server.hpp"
// #include "confFile.hpp"
// #include "Request.hpp"
// #include "Response.hpp"

ssize_t ft_writeFds(int fd, char *buf, size_t size, const char *info, fd_set *fdset);
int ftOpen(std::string name);
ssize_t ft_write(int fd, char *buf, size_t size, const char *info);
ssize_t ft_read(int fd, char *buf, size_t size, const char *info);
ssize_t ft_recv(int fd, char *buf, size_t size, const char *info);
ssize_t ft_send(int fd, char *buf, size_t size, const char *info);
int ft_accept(int socket, struct sockaddr *address, socklen_t*address_len);
int ft_open(const char *path, int flag);
char *lookFor(char *haystack, const char *needle, ssize_t needLen, ssize_t len);
void bWrite(char *buff, ssize_t size);
char *lookForLast(char *haystack, const char *needle, ssize_t needLen, ssize_t len);
ssize_t hexToi(const char *str);
bool fileExists(std::string &name);
size_t fd_size(int fd);
template <typename T, typename U>
bool keyInMap(std::map<T, U> m, T key){
	for (typename std::map<T, U>::iterator it = m.begin(); it != m.end(); it++){
		if (it->first == key)
			return true;
	}
	return false;
}

template <typename T, typename U>
void printMap(std::map<T, U> m){
	std::cerr << "-------------------\n";
	for (typename std::map<T, U>::iterator it = m.begin(); it != m.end(); it++)
		std::cout << it->first << " : " << it->second << "\n";
	std::cerr << "-------------------\n";
}

template <typename T>
void tableInit(T table, T ini, int size){
	int j;
	for (int i = 0; i < size; i++){
		for (j = 0; ini[i][j]; j++)
			table[i][j] = ini[i][j];
		table[i][j] = 0;
	}
}

#endif