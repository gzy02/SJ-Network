#include "../../include/my_optarg.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int cnt_split = 0, cnt_recovery = 0; //全局计数器

void handle(int pid)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) //要写-1，表示所有子进程，写pid的话可能导致一些问题（成片子进程销毁，有些没及时回收成为僵尸进程）
    {
        cnt_recovery += 1;
    }
}
int main(int argc, char **argv)
{
    my_daemon(0, 1);      //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL); //设置缓冲区为空，直接输出
    // signal(SIGCHLD, SIG_IGN);
    signal(SIGCHLD, handle); //子进程状态改变时要给父进程一个信号

    struct rlimit nproc_limit;
    getrlimit(RLIMIT_NPROC, &nproc_limit);
    printf("\n当前限制：%d个进程\n最大限制：%d个进程\n", nproc_limit.rlim_cur, nproc_limit.rlim_max);

    struct sysinfo s_info;

    time_t now, start;
    start = time(NULL);
    Myoptarg arg;
    Set_Config(argc, argv, &arg);
    int bias = 20;                  //预留出的进程个数，防止达到软限制
    for (int i = 1; i <= arg.fnum;) // fork fnum个子进程
    {
        now = time(NULL);
        if ((now - start) >= arg.ptime)
        {
            start = now;
            printf("已分裂%d个子进程    已回收%d个子进程\n", cnt_split, cnt_recovery);
        }
        if (sysinfo(&s_info) == 0)
        {
            // printf("%d\n", s_info.freeram / 1024 / 1024);
            if (s_info.freeram / 1024 > arg.size * sizeof(char) * 5 && cnt_split - cnt_recovery < nproc_limit.rlim_cur - bias) // 剩余内存超过arg.size * sizeof(char)的五倍才fork
            {
                int pid = fork();
                if (pid == -1)
                {
                    perror("Fork error");
                    {
                        printf("用户拥有的进程数量超过软限制,此次fork取消\n待系统空闲时将重新执行此次fork\n");
                        --i; //重新fork这个进程
                        --cnt_split;
                        bias += nproc_limit.rlim_cur - (cnt_split - cnt_recovery); // bias加上它们之间的差额
                    }
                }
                else if (pid == 0) //子进程
                {
                    prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
                    char *str = (char *)malloc(arg.size * 1024 * sizeof(char));
                    if (str == NULL)
                    {
                        ERR_EXIT("malloc error");
                    }
                    //本题必须随机赋值
                    {
                        srand((unsigned)time(NULL));
                        for (int k = 0; k < arg.size * 1024; ++k)
                            str[k] = rand() % 256;
                    }
                    sleep(1); // 20
                    exit(0);
                }
                else //父进程
                {
                    ++cnt_split;
                    ++i;
                }
            }
        }
    }

    while (1)
    {
        printf("已分裂%d个子进程    已回收%d个子进程\n", cnt_split, cnt_recovery);
        sleep(arg.ptime); //要sleep
    }
    return 0;
}
// ./test7 --fnum 50000 --size 1024 --ptime 10
