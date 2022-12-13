/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rezzahra <rezzahra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 23:20:53 by rezzahra          #+#    #+#             */
/*   Updated: 2022/12/12 23:47:55 by rezzahra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

static int checkPermission(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    struct stat st;
    stat(fpath, &st);
    if (!(st.st_mode & S_IWUSR))
        return -3;
    return 0;
}

static int delete1(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);
    if (rv)
        perror(fpath);
    return rv;
}

static int walkTree(char *path)
{
    return nftw(path, checkPermission, 64, FTW_DEPTH | FTW_PHYS);
}

static int walkNdelete(char *path)
{
    return nftw(path, delete1, 64, FTW_DEPTH | FTW_PHYS);
}

void deleteMethod(Response& pons, std::string resource){
    if (!directoryExists(resource) && !fileExists(resource)){
        pons.Code.code = NotFound;
		pons.Code.reason = "Not Found";
		generateErrorHtml(pons);
		pons.isBodyFile = false;
		pons.Code.HTTPv = "HTTP/1.1";
        return ;
    }
    else if (checkPermission(resource.c_str(),NULL,0,NULL) == -3 || walkTree((char *)resource.c_str()) == -3){
        pons.Code.code = Forbidden;
		pons.Code.reason = "Forbidden";
		generateErrorHtml(pons);
		pons.isBodyFile = false;
		pons.Code.HTTPv = "HTTP/1.1";
        return ;
    }
    else if (directoryExists(resource)){
        walkNdelete((char *)resource.c_str());
        pons.Code.code = NoContent;
		pons.Code.reason = "No Content";
		pons.Code.HTTPv = "HTTP/1.1";
        return ;
    }
    else if (fileExists(resource)){
        delete1(resource.c_str(),NULL,0,NULL);
        pons.Code.code = Ok;
		pons.Code.reason = "OK";
		pons.Code.HTTPv = "HTTP/1.1";
        return ;
    }
}

