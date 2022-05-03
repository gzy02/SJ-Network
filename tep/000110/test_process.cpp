#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <unistd.h>
int digit(const char *str)
{
    int s = 0, i = 0;
    while (str[i])
    {
        s *= 10;
        s += str[i] - '0';
        i += 1;
    }
    return s;
}
int main(int argc, char **argv)
{
    setbuf(stdout, NULL);     //设置缓冲区为空，直接输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号
    struct rlimit limit;
    getrlimit(RLIMIT_NPROC, &limit);
    printf("当前限制=%d个进程\n最大限制=%d个进程\n", limit.rlim_cur, limit.rlim_max);
    int num = digit(argv[1]);
    for (int i = 1; i <= num; ++i)
    {
        int pid = fork();
        if (pid == -1)
        {
            printf("fork error\n");
            if (limit.rlim_cur < limit.rlim_max)
            {
                printf("用户拥有的进程数量超过软限制,尝试提升软限制至硬限制\n");
                limit.rlim_cur = limit.rlim_max;
                int tep = setrlimit(RLIMIT_NPROC, &limit);
                if (0 == tep)
                {
                    printf("软限制修改成功\n");
                    printf("当前限制=%d个进程\n最大限制=%d个进程\n", limit.rlim_cur, limit.rlim_max);
                    --i; //重新fork
                }
                else
                {
                    printf("软限制修改失败\n");
                }
            }
            else
            {
                printf("尝试提升硬限制\n");
                limit.rlim_max *= 2;
                limit.rlim_cur *= 2;
                int tep = setrlimit(RLIMIT_NPROC, &limit);
                if (0 == tep)
                {
                    printf("硬限制修改成功\n");
                    printf("当前限制=%d个进程\n最大限制=%d个进程\n", limit.rlim_cur, limit.rlim_max);
                    --i; //重新fork
                }
                else
                {
                    printf("硬限制修改失败\n");
                }
            }
        }
        else if (pid == 0)
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
            sleep(-1);
        }
        else
        {
            printf("已分裂%d个子进程\n", i);
        }
    }
    sleep(-1);
    return 0;
}
