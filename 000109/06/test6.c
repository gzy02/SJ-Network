#include "../../include/my_optarg.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main(int argc, char **argv)
{
    my_daemon(0, 0);          //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL);     //设置缓冲区为空，直接输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号

    Myoptarg arg;
    Set_Config(argc, argv, &arg);
    int fork_error = 0, malloc_error = 0;
    for (int i = 1; i <= arg.fnum; ++i) // fork fnum个子进程
    {
        if (fork_error || malloc_error)
            break;
        int pid = fork();
        if (pid == -1)
        {
            printf("fork error");
            fork_error = 1;
        }
        else if (pid == 0) //子进程
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
            char *str = (char *)malloc(arg.size * 1024 * sizeof(char));
            if (str == NULL)
            {
                printf("malloc error");
                malloc_error = 1;
            }
            else if (arg.init == 1)
            {
                srand((unsigned)time(NULL));
                for (int k = 0; k < arg.size * 1024; ++k)
                    str[k] = rand() % 256;
            }
            while (1)
                sleep(1);
        }
        else //父进程
        {
            if (i % arg.pnum == 0)
                printf("\n已分裂%d个子进程", i);
        }
    }
    if (fork_error == 1)
        printf("fork error");
    if (malloc_error == 1)
        printf("malloc error");
    while (1)
        sleep(1); //第六题貌似要sleep
    return 0;
}
// ./test6 --fnum 1000 --size 1024 --init yes --pnum 100
// ./test6 --fnum 1000 --size 5120 --init yes --pnum 100
// ./test6 --fnum 1000 --size 1024 --init no --pnum 100
// ./test6 --fnum 1000 --size 5120 --init no --pnum 100
