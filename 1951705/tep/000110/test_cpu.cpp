#include <signal.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <time.h>
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
void get_time(int pid, int type, int time_value)
{
    time_t start = time(NULL), now;
    if (!type)
    {
        for (int i = 1; i <= time_value;)
        {
            now = time(NULL);
            if ((int)(now - start) >= 1)
            {
                start = now;
                if (i % 10 == 0)
                {
                    printf("pid=%d,running %d s\n", pid, i);
                }
                ++i;
            }
        }
    }
    else
    {
        for (int i = 1; i <= time_value; ++i)
        {
            if (i % 10 == 0)
                printf("pid=%d,sleep %d s\n", pid, i);
            sleep(1);
        }
    }
}
int main(int argc, char **argv)
{
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号
    setbuf(stdout, NULL);
    int time_value = digit(argv[1]);
    int type = digit(argv[2]);
    int num = digit(argv[3]);
    for (int i = 0; i < num; ++i)
    {
        int pid = fork();
        if (pid == -1)
            printf("fork error\n");
        else if (pid == 0)
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
            get_time(getpid(), type, time_value);
        }
        else
        {
            get_time(getpid(), type, time_value);
        }
    }

    printf("OVER\n");
    return 0;
}
