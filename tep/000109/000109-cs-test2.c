#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
void my_daemon(int nochdir, int noclose);

int main()
{
    char path[100];
    getcwd(path, 100);
    my_daemon(0, 1);
    setbuf(stdout, NULL);
    int fd;
    fd = open(strcat(path, "/my_daemon.log"), O_WRONLY | O_CREAT | O_APPEND, 0644);
    const char *buf = "1951705¸ßÔøÒê\n";
    if (fd == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }
    time_t now, start = time(NULL);
    while (1)
    {
        now = time(NULL);
        if (now - start >= 5)
        {
            start = now;
            printf("%s", buf);
            write(fd, buf, strlen(buf));
        }
        // sleep(5);
    }
    close(fd);
    return 0;
}
