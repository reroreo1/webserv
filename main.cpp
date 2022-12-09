/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rezzahra <rezzahra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 23:20:53 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/09 18:01:25 by rezzahra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

int main(){
	rmrf("./pop");
}
// void Delete1(std::string path){
// 	DIR* d = opendir(path.c_str());
// 	struct dirent *dp;
// 	while((dp = readdir(d))!= NULL){
// 		if (dp->d_type & DT_DIR){
// 			if (strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
// 				char path1[2000] = {0};
// 				strcat(path1,path.c_str());
// 				strcat(path1,"/");
// 				strcat(path1,dp->d_name);
// 				std::cout << path1 << std::endl;
// 				Delete1(std::string(path1));
// 			}
// 		}
// 		else
// 			unlink(dp->d_name);
// 	}
// }
// int main(){
// 	Delete1("./pop");
// }