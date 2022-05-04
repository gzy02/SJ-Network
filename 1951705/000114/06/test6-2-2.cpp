#include "../../include/my_daemon.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
int SetNonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (-1 == flags)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void SetNonblock(int socket_fd)
{

    if (SetNonblocking(socket_fd) == -1)
    {
        printf("Error: SetNonblock()\n");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    my_daemon(1, 1);
    const char *filename = "uds-tmp";
    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, filename);

    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("Request socket failed\n");
        return -1;
    }
    SetNonblock(sock_fd);

    char buffer[BUFFER_SIZE];
    while (1)
    {
        int ret = connect(sock_fd, (struct sockaddr *)&un, sizeof(un));
        if (ret == -1 && !(errno == EINPROGRESS || errno == EALREADY))
        {
            printf("connect error:%s\n", strerror(errno));
            exit(-1);
        }
        else if (ret != -1)
        {
            printf("connect to %s success\n", un.sun_path);
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, "1951705 高曾谊\n");
            while (1)
            {
                int num = send(sock_fd, buffer, BUFFER_SIZE, 0);
                if (num == -1 && errno != EAGAIN)
                {
                    printf("send error:%d->%s", errno, strerror(errno));
                    exit(-1);
                }
                else if (num == 0)
                {
                    printf("disconnect\n");
                    exit(0);
                }
                else if (num > 0) //正常send到了
                {
                    printf("Client sent.\nData: %s\n", buffer);
                    while (1)
                    {
                        num = recv(sock_fd, buffer, BUFFER_SIZE, 0);
                        if (num == -1 && errno != EAGAIN)
                        {
                            printf("send error:%d->%s", errno, strerror(errno));
                            exit(-1);
                        }
                        else if (num == 0)
                        {
                            printf("disconnect\n");
                            exit(0);
                        }
                        else if (num > 0) //正常received到了
                        {
                            printf("Client received.\nData: %s\n", buffer);
                            break;
                        }
                        sleep(1);
                    }
                    break;
                }
                sleep(1);
            }
            break;
        }
        sleep(1);
    }

    close(sock_fd);
    return 0;
}
