#include "../../include/my_daemon.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //errno
#include <fcntl.h>  //O_
#include <iostream> //cout
#include <string>   //string
#include <unistd.h> //sleep
using namespace std;
const int BUF_SIZE = 1024;

int lock_set(int fd, int type); //上锁/解锁

int main(int argc, char **argv)
{
    my_daemon(1, 1); //切换目录至根目录，不关文件描述符
    Socket_optarg arg(argc, argv);
    arg.Print_Config();

    int fd = open(arg.filename.c_str(), O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        ERR_EXIT("Error: Open");
    }

    char buf[BUF_SIZE];
    int ret = 0, file_non_empty = 0;

    while (file_non_empty == 0)
    {
        ret = 0;

        while (ret == 0) //需要写，反复申请写锁
        {
            ret = lock_set(fd, F_RDLCK); //读锁
            sleep(1);
        }
        memset(buf, 0, sizeof(buf));
        ret = read(fd, buf, sizeof(buf));
        if (ret == 0)
        {
            printf("File empty!\n");
            while (!ret)
            {
                ret = lock_set(fd, F_UNLCK);
                sleep(1);
            }
        }
        else
        {
            file_non_empty = 1;
            printf("%s\n", buf);
            while (1)
            {
                ret = read(fd, buf, BUF_SIZE);
                if (ret > 0)
                {
                    printf("%s\n", buf);
                }
                else //读完了
                    break;
            }
            printf("File read finished\n");
            ret = 0;
            while (ret == 0)
            {
                ret = lock_set(fd, F_UNLCK);
                sleep(1);
            }
        }
        sleep(1);
    }

    close(fd);
    return 0;
}
