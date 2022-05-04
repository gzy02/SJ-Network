#include "../../include/Socket_Connect.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //errno
#include <iostream> //cout
#include <unistd.h> //sleep

#include <sys/select.h> //select
#include <sys/time.h>
#include <sys/types.h>
using namespace std;
int main(int argc, char **argv)
{
     Socket_optarg arg;
     arg.Set_Config(argc, argv);
     Socket_Connect cur_connect(arg, NONBLOCK);
     int cur_fd = cur_connect.Getsocketfd();
     fd_set rset, allset;
     FD_ZERO(&allset);
     FD_SET(cur_fd, &allset);
     rset = allset;
     int ready_num = select(cur_fd + 1, &rset, NULL, NULL, NULL);
     cout << "ready_num: " << ready_num << endl;
     cout << errno << endl
          << strerror(errno) << endl;
     ssize_t len = cur_connect.Read();
     cout << "len: " << len << endl;
     cout << errno << endl
          << strerror(errno) << endl;
     return 0;
}
