#include "../../include/my_daemon.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_CONNECT_NUM 2
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

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Request socket error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }
    SetNonblock(fd);

    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    unlink(filename);
    strcpy(un.sun_path, filename);
    if (bind(fd, (struct sockaddr *)&un, sizeof(un)) < 0)
    {
        printf("Bind error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }
    if (listen(fd, MAX_CONNECT_NUM) < 0)
    {
        printf("Listen error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }
    printf("Listening...\n");

    char buffer[BUFFER_SIZE], newbuffer[BUFFER_SIZE];

    int new_fd;
    while (1)
    {
        new_fd = accept(fd, NULL, NULL);
        if (new_fd == -1 && errno != EAGAIN)
        {

            printf("Accpet error:%s\n", strerror(errno));
            exit(-1);
        }
        else if (new_fd != -1) // accept到了
        {
            printf("Connect SUCCESS\n");
            SetNonblock(new_fd);
            memset(buffer, 0, BUFFER_SIZE);
            while (1)
            {
                int ret = recv(new_fd, buffer, BUFFER_SIZE, 0);
                if (ret < 0 && errno != EAGAIN)
                {

                    printf("Recv error:%d->%s\n", errno, strerror(errno));
                    exit(-1);
                }
                else if (ret == 0)
                {
                    printf("disconnect\n");
                    exit(0);
                }
                else if (ret > 0) //正常recv到了
                {
                    memset(newbuffer, 0, BUFFER_SIZE);
                    sprintf(newbuffer, "Server received.\nYour data: %s\n", buffer);
                    printf("%s\n", newbuffer);

                    while (1)
                    {
                        ret = send(new_fd, buffer, BUFFER_SIZE, 0);
                        if (ret < 0)
                        {
                            if (errno != EAGAIN)
                            {
                                printf("send error:%d->%s\n", errno, strerror(errno));
                                exit(-1);
                            }
                        }
                        else if (ret == 0)
                        {
                            printf("disconnect\n");
                            exit(0);
                        }
                        else
                        {
                            printf("Server sent.\nData: %s\n", buffer);
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

    close(new_fd);
    close(fd);
    unlink(filename);

    return 0;
}
