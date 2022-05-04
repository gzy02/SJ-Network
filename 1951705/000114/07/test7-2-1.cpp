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

int lock_set(int fd, int type); //����/����

int main(int argc, char **argv)
{
    my_daemon(1, 1); //�л�Ŀ¼����Ŀ¼�������ļ�������
    Socket_optarg arg(argc, argv);
    arg.Print_Config();

    int fd = open(arg.filename.c_str(), O_RDWR | O_CREAT | O_NONBLOCK, 0644);
    if (fd < 0)
    {
        ERR_EXIT("Error: Open");
    }
    char buf[BUF_SIZE];
    strcpy(buf, "1951705 ������ test7-1-1");
    cout << buf << endl;

    int ret = 0;
    while (ret == 0) //��Ҫд����������д��
    {
        ret = lock_set(fd, F_WRLCK); //д��
        sleep(1);
    }
    int len = write(fd, buf, strlen(buf));
    if (len < 0)
        ERR_EXIT("Error: Write");
    ret = 0;
    while (ret == 0)
    {
        ret = lock_set(fd, F_UNLCK);
        sleep(1);
    }
    close(fd);

    return 0;
}
