#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
void ERR_EXIT(const char *m)
{
    perror(m);
    exit(EXIT_FAILURE);
}

void my_daemon(int nochdir, int noclose)
{
    pid_t pid;
    pid = fork();
    if (pid == -1)
        ERR_EXIT("fork error");
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() == -1)
        ERR_EXIT("SETSID ERROR");
    if (nochdir == 0)
        chdir("/");
    if (noclose == 0)
    {
        int i;
        for (i = 0; i < 3; ++i) //���ļ�������stdin ����stdout stderr ��Ϊ3��ȫ��
        {
            close(i);
            open("/dev/null", O_RDWR);
        }
    }
    umask(0);
    return;
}
