#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/socket_optarg.h"
#include <climits>
#include <cstdlib>      //exit
#include <cstring>      //strerror
#include <errno.h>      //error
#include <iostream>     //cout
#include <sys/select.h> //select
#include <sys/time.h>
#include <sys/types.h>
using namespace std;
int main(int argc, char **argv)
{

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, NONBLOCK); // listen(fd,1)
    fd_set rset, allset;
    int max_fd = INT_MIN;
    FD_ZERO(&allset);
    while (1)
    {
        int cur_fd = -1;
        while (cur_fd == -1)
        {
            cur_fd = cur_connect.AcceptClient(); //等Client连接
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                cerr << "Accept Error" << endl;
                cerr << errno << endl
                     << strerror(errno) << endl;
                exit(EXIT_FAILURE);
            }
        }
        cur_connect.Setaccept_fd_nonblock(cur_fd);

        FD_SET(cur_fd, &allset);
        rset = allset;
        max_fd = max(max_fd, cur_fd);

        // 最后一个参数 不等待
        timeval t;
        t.tv_sec = 0;
        t.tv_usec = 0;
        int ready_num = select(max_fd + 1, &rset, NULL, NULL, &t);
        // int ready_num = select(cur_fd + 1, &rset, NULL, NULL, NULL);
        cout << "ready_num: " << ready_num << endl;
        cout << errno << endl
             << strerror(errno) << endl;

        ssize_t len = cur_connect.Read(cur_fd);
        cout << "len: " << len << endl;

        cout << errno << endl
             << strerror(errno) << endl;
        // FD_CLR(cur_fd, &allset);
    }

    return 0;
}
