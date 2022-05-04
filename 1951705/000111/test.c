#include "readconfig.c"
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define NAME_LENGTH 128 //进程名最大长度

void handle(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) //回收所有子进程
        ;
}

int main(int argc, char **argv, char **env)
{
    my_daemon_with_console(0, 1); //带控制台的伪守护进程
    char value[MAXN];
    read_dat("/home/stu/u1951705/1951705.dat", value);
    printf("%s", value);
    int num;
    num = read_conf("/etc/stu/stu_1951705.conf");
    // printf("子进程数量=%d\n", num);

    int sec, hour, min, all;
    time_t start = time(NULL), now; //父进程开始计时

    signal(SIGCHLD, handle);

    int *flags = (int *)malloc(sizeof(int) * num); //存储子进程pid号
    if (flags == NULL)
    {
        perror("malloc error\n");
        exit(-1);
    }
    for (int i = 0; i < num; ++i)
    {
        flags[i] = 0;
    }

    char newfullname[NAME_LENGTH];
    char childname[NAME_LENGTH];
    char originname[NAME_LENGTH];
    strcpy(originname, argv[0]);    //备份argv[0]
    char tepshortname[NAME_LENGTH]; //临时缓冲区，用于存储新名字

    char newshortname[NAME_LENGTH];
    memset(tepshortname, 0, sizeof(tepshortname));
    sprintf(newshortname, strcat(strcpy(tepshortname, originname), " [main]"));
    prctl(PR_SET_NAME, newshortname); // ps指令（Shell的所有作业）改进程名字

    while (1)
    {
        for (int i = 0; i < num; ++i)
        {
            sprintf(childname, "/proc/%d", flags[i]);
            if (flags[i] == 0 || access(childname, 0) == -1) // flags未初始化 或子进程不存在(看看能不能访问/proc/{flags[i]}即可知道)
            {
                pid_t pid;
                while ((pid = fork()) == -1)
                {
                    perror("Fork error.\n");
                    sleep(1);
                }
                if (pid == 0)
                {
                    // prctl(PR_SET_PDEATHSIG, SIGKILL); //子进程接受父进程的死亡信号被杀后
                    //但是pdf里面写了不能杀子进程，只能另辟蹊径getppid()判断自己是否是孤儿进程
                    start = time(NULL); //重新计数
                    memset(tepshortname, 0, sizeof(tepshortname));
                    sprintf(newshortname, strcat(strcpy(tepshortname, originname), " [Sub-%02d]"), i + 1);
                    prctl(PR_SET_NAME, newshortname);

                    while (1)
                    {
                        if (getppid() == 1) //发现自己是孤儿进程，自己退出了（确实没有杀子进程
                        {
                            exit(0);
                        }
                        now = time(NULL);
                        all = now - start;
                        sec = all % 60;
                        min = (all / 60) % 60;
                        hour = all / 3600;
                        memset(tepshortname, 0, sizeof(tepshortname));
                        sprintf(newfullname, strcat(strcpy(tepshortname, originname), " [Sub-%02d %02d:%02d:%02d]"), i + 1, hour, min, sec);
                        strcpy(argv[0], newfullname); // ps -ef的进程名修改

                        sleep(1); //死循环附带sleep
                    }
                    exit(0);
                }
                else //父进程
                {
                    flags[i] = pid;
                    break;
                }
            }
        }

        //主进程
        now = time(NULL);
        all = now - start;
        sec = all % 60;
        min = (all / 60) % 60;
        hour = all / 3600;
        memset(tepshortname, 0, sizeof(tepshortname));
        sprintf(newfullname, strcat(strcpy(tepshortname, originname), " [main %02d:%02d:%02d]"), hour, min, sec);
        strcpy(argv[0], newfullname);

        sleep(1); //死循环附带sleep
    }
    free(flags);
    return 0;
}
