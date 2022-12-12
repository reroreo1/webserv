/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rezzahra <rezzahra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 23:20:53 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/11 16:54:43 by rezzahra         ###   ########.fr       */
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

int main(){
	
}