#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //error
#include <iostream> //cout

#include <sys/select.h> //select
#include <sys/time.h>
#include <sys/types.h>
using namespace std;
int main(int argc, char **argv)
{

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); // listen(fd,1)

    int cur_fd = cur_connect.AcceptClient(); //等Client连接
    cur_connect.Setaccept_fd_nonblock(cur_fd);

    fd_set rset, allset;
    FD_ZERO(&allset);
    FD_SET(cur_fd, &allset);
    rset = allset;

    // 最后一个参数为NULL表示等待无限时间
    int ready_num = select(cur_fd + 1, &rset, NULL, NULL, NULL);
    cout << "ready_num: " << ready_num << endl;
    cout << errno << endl
         << strerror(errno) << endl;

    ssize_t len = cur_connect.Read(cur_fd);
    cout << "len: " << len << endl;

    cout << errno << endl
         << strerror(errno) << endl;

    return 0;
}
