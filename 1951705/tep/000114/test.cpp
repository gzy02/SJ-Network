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
    const int LEN = 700000;
    if (pid == -1)
        ERR_EXIT("Error: Fork");
    else if (pid == 0) //�ӽ���
    {
        prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
        close(fd[1]);                     //�ӽ��̲�дֻ��
        const int readlen = 70000;
        char buf[readlen + 1];
        while (fd[0] != -1)
            cout << "read_cnt:" << read(fd[0], buf, readlen) << endl;
        // cout << "Child process(pid:" << getpid() << ") has read from parent process(pid:" << getppid() << ") ->" << buf << endl;

        while (1)
            sleep(1);
        exit(EXIT_SUCCESS);
    }
    else //������
    {
        close(fd[0]); //������ֻд����
        string send_str(LEN, 'A');
        int write_cnt = write(fd[1], send_str.c_str(), send_str.length());
        cout << "write_cnt:" << write_cnt << endl;
        // cout << "Parent process(pid:" << getpid() << ") has written to child process(pid:" << pid << ") ->" << send_str << endl;
        while (1)
            sleep(1);
    }
    return 0;
}
