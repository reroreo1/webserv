#include "globalheader.hpp"

static bool fileExists(std::string &name){
	bool ret;
	std::ifstream f(name.c_str());
	ret = f.good();
	f.close();
	return ret;
}

ssize_t ft_writeFds(int fd, char *buf, size_t size, const char *info, fd_set *fdset){
	ssize_t rt;
	FD_SET(fd, fdset);
	if ((rt = write(fd, buf, size)) == -1){
		perror(info);
	};
	return (rt);
	FD_SET(fd, fdset);
}

int ftOpen(std::string name){
	int ret;
	ret = fileExists(name) ? open(name.c_str(), O_RDWR | O_TRUNC) : open(name.c_str(), O_CREAT | O_RDWR, 0666);
	if (ret == -1){
		perror("creating.");
		exit(-1);
	}
	return ret;
}

ssize_t ft_write(int fd, char *buf, size_t size, const char *info){
	ssize_t rt;
	if ((rt = write(fd, buf, size)) == -1){
		perror(info);
	};
	return (rt);
}

ssize_t ft_read(int fd, char *buf, size_t size, const char *info){
	ssize_t rt;
	if ((rt = read(fd, buf, size)) == -1){
		if (info)
			perror(info);
	};
	return (rt);
}

ssize_t ft_recv(int fd, char *buf, size_t size, const char *info){
	ssize_t rt;
	if ((rt = recv(fd, buf, size, 0)) == -1){
		perror(info);
	};
	return (rt);
}

ssize_t ft_send(int fd, char *buf, size_t size, const char *info){
	ssize_t rt;
	if ((rt = send(fd, buf, size, 0)) == -1){
		perror(info);
	};
	return (rt);
}

int ft_accept(int socket, struct sockaddr *address, socklen_t*address_len){
	int rt;
	if ((rt = accept(socket, address, address_len)) == -1){
		perror("From accept: ");
	};
	return (rt);
}

int ft_open(const char *path, int flag){
	int rt;
	if ((rt = open(path, flag)) == -1){
		perror("From accept: ");
	};
	return (rt);
}

char	*lookFor(char *haystack, const char *needle, ssize_t needLen, ssize_t len){
	if (len < needLen)
		return NULL;
	for (ssize_t i = 0; i <= len - needLen; i++) {
		if(!memcmp(&haystack[i], needle, needLen))
			return (&(haystack[i]));
	}
	return NULL;
}

char	*lookForLast(char *haystack, const char *needle, ssize_t needLen, ssize_t len){
	if (len < needLen)
		return NULL;
	for (ssize_t i = len - needLen; i >= 0; i--) {
		if(!memcmp(&haystack[i], needle, needLen))
			return (&(haystack[i]));
	}
	return NULL;
}

void bWrite(char *buff, ssize_t size){
	for (int i = 0; i < size; i++){
		if (buff[i] >= 32 && buff[i] <= 126)
			printf("%c", buff[i]);
		else if (buff[i] == 13)
			printf("|r|");
		else
			printf("|%d|", buff[i]);
	}
}

static int indexInStr(char *str, char c){
	for (int i = 0; str[i]; i++){
		if (str[i] == c)
			return i;
	}
	return -1;
}

ssize_t hexToi(const char *str){
	ssize_t res = 0;
	char base[]  = {"0123456789abcdef"};
	for (int i = 0; str[i]; i++){
		if (indexInStr(base, str[i]) == -1){
			bWrite((char *) str, strlen(str));
			std::cerr << "somthing in hexToi\n";
			exit(1);
		}
		res = res * 16 + indexInStr(base, str[i]);
	}
	return res;
}


// int main(){
// 	std::map<int, std::string> m;

// 	m.insert(std::make_pair<int, std::string>(1, "one"));
// 	m.insert(std::make_pair<int, std::string>(2, "two"));
// 	m.insert(std::make_pair<int, std::string>(3, "three"));

// 	std::cout << keyInMap(m, 1) << std::endl;
// 	std::cout << keyInMap(m, 5) << std::endl;
// }