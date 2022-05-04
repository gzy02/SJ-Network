#include "../../include/my_optarg.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main(int argc, char **argv)
{
    my_daemon(0, 1);          //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL);     //设置缓冲区为空，直接输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号

    Myoptarg arg;
    Set_Config(argc, argv, &arg);

    for (int i = 1; i <= arg.fnum; ++i) // fork fnum个子进程
    {
        int pid = fork();
        if (pid == -1)
        {
            ERR_EXIT("fork error");
        }
        else if (pid == 0) //子进程
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
            char *str = (char *)malloc(arg.size * 1024 * sizeof(char));
            while (1)
            {
                ;
            }
        }
        else //父进程
        {
            if (i % arg.pnum == 0)
                printf("已分裂%d个子进程", i);
        }
    }
    while (1)
    {
        ;
    }

    return 0;
}
