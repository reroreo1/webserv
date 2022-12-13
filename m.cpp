#include "Response.hpp"

int checkPermission(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    struct stat st;
    stat(fpath, &st);
    if (!(st.st_mode & S_IWUSR))
        return -1;
    return 0;
}

int main(){
	std::cout << checkPermission("./helperFile",NULL,0,NULL);
}