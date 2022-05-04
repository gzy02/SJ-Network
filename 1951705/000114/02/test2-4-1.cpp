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
    my_daemon(1, 1); //目录依然是本目录，不关文件描述符

    int ret = mkfifo("myfifo", 0666);
    if (ret == -1)
    {
        cout << errno << strerror(errno) << endl;
        if (errno != 17) //文件已存在
            ERR_EXIT("Error: Mkfifo");
    }
    int write_fd = open("myfifo", O_WRONLY);
    if (write_fd < 0)
        ERR_EXIT("Error: Open");
    const int LEN = 800000;
    string send_str(LEN, 'a');
    cout << "begin" << endl;
    int write_cnt = write(write_fd, send_str.c_str(), send_str.length());
    cout << "写入数据大小:" << write_cnt << endl;
    if (write_cnt == -1)
        ERR_EXIT("Error: Write");
    close(write_fd);

    return 0;
}
