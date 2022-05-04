#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //error
#include <iostream> //cout
using namespace std;
int main(int argc, char **argv)
{

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); //传入notaccept防阻塞，仅初始化
    cur_connect.Setreadbytes(20);             //设置为20字节
    while (true)
    {
        int cur_fd = cur_connect.AcceptClient(); //等Client连接
        if (cur_fd == -1)
        {
            cout << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        ssize_t len = cur_connect.Read(cur_fd);
        cout << "len: " << len << endl;

        cout << errno << endl
             << strerror(errno) << endl;

        cout << "Recv_str:" << cur_connect.Getread_str() << endl;
    }
    return 0;
}
