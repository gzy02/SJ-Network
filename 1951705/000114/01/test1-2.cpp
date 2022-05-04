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
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //��������Ϊ0���������
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�
    int fd[2];
    int ret = pipe(fd); //������һ����������ͨ�������ͨ�����������ڽ���֮��ͨ�š�
    // 1����Ҫʹ��pipefd[0]��д���ݣ�Ҳ��Ҫʹ��pipefd[1]����ȡ���ݣ���������Ϊ��δ����ġ�
    // 2������pipefd[0]��������ʱ��Ҫ�ȹر�pipefd[1];����pipefd[1]��д������ʱ��Ҫ�ر�pipefd[0]��
    if (ret == -1)
        ERR_EXIT("Error: Pipe");

    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("Error: Fork");

    else if (pid == 0) //�ӽ���
    {
        prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
        close(fd[0]);                     //�ӽ��̲���ֻд
        string send_str = "1951705 ������\n";
        if (write(fd[1], send_str.c_str(), send_str.length()) == -1)
            ERR_EXIT("Error: Write");

        cout << "Child process(pid:" << getpid() << ") has written to parent process(pid:" << getppid() << ") ->" << send_str << endl;

        while (1)
            sleep(1);
        exit(EXIT_SUCCESS);
    }
    else //������
    {
        close(fd[1]); //������ֻ����д
        const int len = 1024;
        char buf[len + 1];

        if (read(fd[0], buf, len) == -1)
            ERR_EXIT("Error: Read");

        cout << "Parent process(pid:" << getpid() << ") has read from child process(pid:" << pid << ") ->" << buf << endl;

        while (1)
            sleep(1);
    }
    return 0;
}
