#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main()
{
    my_daemon(0, 1); //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL);
    signal(SIGCHLD, SIG_IGN);
    for (int i = 0; i < 10; ++i) // fork十个子进程
    {
        sleep(3);
        int pid = fork();
        if (pid == -1)
        {
            ERR_EXIT("fork error");
        }
        else if (pid == 0) //子进程
        {
            while (1) //打印
            {
                printf("%d %d 1951705 sub\n", getppid(), getpid());
                sleep(25);
            }
            // exit(EXIT_SUCCESS);
        }
    }
    while (1)
    {
        printf("%d %d 1951705 main\n", getppid(), getpid());
        sleep(5);
    }

    return 0;
}
