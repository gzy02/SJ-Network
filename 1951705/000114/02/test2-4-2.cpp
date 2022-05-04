#include "../../include/my_daemon.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //errno
#include <fcntl.h>  //O_
#include <iostream> //cout
#include <signal.h> //signal
#include <string>
#include <sys/prctl.h> // prctl
#include <sys/stat.h>  //mkfifo
#include <sys/types.h> //
#include <unistd.h>    //sleep
using namespace std;
int main(int argc, char **argv)
{
    my_daemon(1, 1);          //目录依然是本目录，不关文件描述符

    int ret = mkfifo("myfifo", 0666);
    if (ret == -1)
    {
        cout << errno << strerror(errno) << endl;
        if (errno != 17) //文件已存在
            ERR_EXIT("Error: Mkfifo");
    }

    int read_fd = open("myfifo", O_RDONLY);
    if (read_fd < 0)
        ERR_EXIT("Error: Open");
    const int LEN = 800000;
    char buf[LEN];
    int read_cnt = read(read_fd, buf, sizeof(buf));
    cout << "读入数据大小:" << read_cnt << endl;
    //cout << buf[0] << buf[2] << endl;
    if (read_cnt == -1)
        ERR_EXIT("Error: Read");
    close(read_fd);

    return 0;
}
