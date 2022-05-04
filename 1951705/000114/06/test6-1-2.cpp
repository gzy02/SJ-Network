#include "../../include/my_daemon.h"
#include <errno.h>
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
int main()
{
    my_daemon(1, 1);
    const char *filename = "uds-tmp"; //不需要指定ip和port，指定文件路径就行
    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, filename);

    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("Request socket failed\n");
        return -1;
    }
    if (connect(sock_fd, (struct sockaddr *)&un, sizeof(un)) < 0)
    {
        printf("connect socket failed\n");
        return -1;
    }
    printf("connect to %s success\n", un.sun_path);

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "1951705 高曾谊\n");
    int num = send(sock_fd, buffer, BUFFER_SIZE, 0);
    printf("Client sent.\nData: %s\n", buffer);
    if (num < 0)
    {
        printf("Send error:%d->%s", errno, strerror(errno));
        exit(-1);
    }
    num = recv(sock_fd, buffer, BUFFER_SIZE, 0);
    if (num < 0)
    {
        printf("Recv error:%d->%s", errno, strerror(errno));
        exit(-1);
    }
    printf("Client received.\nData: %s\n", buffer);
    close(sock_fd);

    return 0;
}
