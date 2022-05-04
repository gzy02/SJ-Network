#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/my_daemon.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>     //exit
#include <cstring>     //strerror
#include <errno.h>     //error
#include <iostream>    //cout
#include <signal.h>    //signal
#include <sys/prctl.h> // prctl
#include <unistd.h>    //fork
using namespace std;

int main(int argc, char **argv)
{
    my_daemon(0, 1);          //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL);     //缓冲区设为0，立刻输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); //
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
        int pid = fork();
        if (pid == -1)
            ERR_EXIT("Error: Fork");
        else if (pid == 0) //子进程
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
            ssize_t len = cur_connect.Read(cur_fd);
            cout << "len: " << len << endl;

            cout << errno << endl
                 << strerror(errno) << endl;

            cout << "Recv_str:" << cur_connect.Getread_str() << endl;
            exit(EXIT_SUCCESS);
        }
        else //父进程进入下轮循环继续等accept
            continue;
    }
    return 0;
}
