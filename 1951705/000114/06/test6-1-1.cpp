#include "../../include/my_daemon.h"
#include <errno.h>
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
int main()
{
    my_daemon(1, 1);

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Request socket error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }

    struct sockaddr_un un;
    const char *filename = "uds-tmp"; //本目录这个文件
    un.sun_family = AF_UNIX;
    unlink(filename); //先删掉，也可在bind时检测errno
    strcpy(un.sun_path, filename);

    if (bind(fd, (struct sockaddr *)&un, sizeof(un)) < 0) //创建文件
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

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int new_fd = accept(fd, NULL, NULL);
    if (new_fd < 0)
    {
        printf("Accept error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }

    int ret = recv(new_fd, buffer, BUFFER_SIZE, 0);
    if (ret < 0)
    {
        printf("Recv error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }
    char newbuffer[BUFFER_SIZE];
    memset(newbuffer, 0, sizeof(newbuffer));
    sprintf(newbuffer, "Server received.\nYour data: %s\n", buffer);
    printf("%s\n", newbuffer);

    ret = send(new_fd, newbuffer, BUFFER_SIZE, 0);
    if (ret < 0)
    {
        printf("Send error:%d->%s\n", errno, strerror(errno));
        exit(-1);
    }
    printf("Server sent.\nData: %s\n", buffer);

    close(new_fd);
    close(fd);
    unlink(filename); //最后再删掉

    return 0;
}
