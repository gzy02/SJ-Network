#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main()
{
    char path[100];
    getcwd(path, 100);
    my_daemon(0, 1); //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);
    int fd;
    fd = open(strcat(path, "/my_daemon.log"), O_WRONLY | O_CREAT | O_APPEND, 0644);
    const char *buf = "1951705������\n";
    if (fd == -1)
    {
        ERR_EXIT("open error");
    }
    while (1)
    {
        printf("%s", buf);
        write(fd, buf, strlen(buf));
        sleep(5);
    }
    close(fd);
    return 0;
}
