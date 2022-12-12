/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 23:20:53 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/12 19:02:39 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

int unlink_cb1(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    struct stat st;
    stat(fpath, &st);
    if (!(st.st_mode & S_IWUSR))
        return -1;
    return 0;
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);
    if (rv)
        perror(fpath);
    return rv;
}

int rmrf(char *path)
{
    return nftw(path, unlink_cb1, 64, FTW_DEPTH | FTW_PHYS);
}
int rmrf1(char *path)
{
    return nftw(path, unlink_cb1, 64, FTW_DEPTH | FTW_PHYS);
}


int main(){
	std::cout << rmrf("./helperfile") << std::endl;
}
