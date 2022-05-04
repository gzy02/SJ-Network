#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main()
{
    my_daemon(0, 1);          //切换目录至根目录，不关文件描述符
    signal(SIGCHLD, SIG_IGN); // 75s
    setbuf(stdout, NULL);
    for (int i = 0; i < 10; ++i) // fork十个子进程
    {
        sleep(3);
        int pid = fork();
        int status = -1;
        if (pid == -1)
        {
            ERR_EXIT("fork error");
        }
        else if (pid == 0) //子进程
        {
            for (int j = 0; j < 3; ++j) //打印三次
            {
                printf("%d %d 1951705 sub\n", getppid(), getpid());
                sleep(25);
            }
            exit(EXIT_SUCCESS);
        }
        // else
        //{
        //     wait(&status); //父进程执行到此，马上阻塞自己，直到有子进程结束。
        //当发现有子进程结束时，就会回收它的资源。 750s
        // }
    }
    while (1)
    {
        printf("%d %d 1951705 main\n", getppid(), getpid());
        sleep(5);
    }

    return 0;
}
