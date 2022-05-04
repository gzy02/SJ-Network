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
    setbuf(stdout, NULL);     //缓冲区设为0，立刻输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号

    int ret = mkfifo("myfifo", 0666);
    if (ret == -1)
    {
        cout << strerror(errno) << endl;
        if (errno != 17) //文件已存在
            ERR_EXIT("Error: Mkfifo");
    }

    int pid = fork();
    if (pid == -1)
        ERR_EXIT("Error: Fork");
    else if (pid == 0) //子进程
    {
        prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
        int read_fd = open("myfifo", O_RDONLY);
        if (read_fd < 0)
            ERR_EXIT("Error: Open");
        char buf[1024];
        if (read(read_fd, buf, sizeof(buf)) == -1)
            ERR_EXIT("Error: Read");
        cout << "Child process(pid:" << getpid() << ") has read from parent process(pid:" << getppid() << ") ->" << buf << endl;

        int write_fd = open("myfifo", O_WRONLY);
        if (write_fd < 0)
            ERR_EXIT("Error: Write");
        string send_str = "1951705 高曾谊";
        if (write(write_fd, send_str.c_str(), send_str.length()) == -1)
            ERR_EXIT("Error: Write");
        cout << "Child process(pid:" << getpid() << ") has written to parent process(pid:" << getppid() << ") ->" << send_str << endl;
        close(write_fd);
        while (1)
            sleep(1);
        exit(EXIT_SUCCESS);
    }
    else //父进程
    {
        int write_fd = open("myfifo", O_WRONLY);
        if (write_fd < 0)
            ERR_EXIT("Error: Write");

        string send_str = "1951705 高曾谊";
        if (write(write_fd, send_str.c_str(), send_str.length()) == -1)
            ERR_EXIT("Error: Write");
        cout << "Parent process(pid:" << getpid() << ") has written to child process(pid:" << pid << ") ->" << send_str << endl;
        close(write_fd);

        int read_fd = open("myfifo", O_RDONLY);
        if (read_fd < 0)
            ERR_EXIT("Error: Open");
        char buf[1024];
        if (read(read_fd, buf, sizeof(buf)) == -1)
            ERR_EXIT("Error: Read");
        cout << "Parent process(pid:" << getpid() << ") has read from child process(pid:" << pid << ") ->" << buf << endl;
        close(read_fd);
        while (1)
            sleep(1);
    }
    return 0;
}
