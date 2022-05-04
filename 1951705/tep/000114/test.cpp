#include "../../include/my_daemon.h"
#include <cstdlib>     //exit
#include <cstring>     //strerror
#include <errno.h>     //errno
#include <iostream>    //cout
#include <signal.h>    //signal
#include <string>      //string
#include <sys/prctl.h> // prctl
#include <unistd.h>    //sleep
using namespace std;
int main(int argc, char **argv)
{
    my_daemon(0, 1);          //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL);     //缓冲区设为0，立刻输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号
    int fd[2];
    int ret = pipe(fd); //创建了一个单向数据通道，这个通道可以用来在进程之间通信。
    // 1）不要使用pipefd[0]来写数据，也不要使用pipefd[1]来读取数据，否则其行为是未定义的。
    // 2）在用pipefd[0]来读数据时，要先关闭pipefd[1];在用pipefd[1]来写入数据时，要关闭pipefd[0]。
    if (ret == -1)
        ERR_EXIT("Error: Pipe");

    pid_t pid = fork();
    const int LEN = 700000;
    if (pid == -1)
        ERR_EXIT("Error: Fork");
    else if (pid == 0) //子进程
    {
        prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
        close(fd[1]);                     //子进程不写只读
        const int readlen = 70000;
        char buf[readlen + 1];
        while (fd[0] != -1)
            cout << "read_cnt:" << read(fd[0], buf, readlen) << endl;
        // cout << "Child process(pid:" << getpid() << ") has read from parent process(pid:" << getppid() << ") ->" << buf << endl;

        while (1)
            sleep(1);
        exit(EXIT_SUCCESS);
    }
    else //父进程
    {
        close(fd[0]); //父进程只写不读
        string send_str(LEN, 'A');
        int write_cnt = write(fd[1], send_str.c_str(), send_str.length());
        cout << "write_cnt:" << write_cnt << endl;
        // cout << "Parent process(pid:" << getpid() << ") has written to child process(pid:" << pid << ") ->" << send_str << endl;
        while (1)
            sleep(1);
    }
    return 0;
}
