#include "../../include/my_daemon.h"
#include <cstdlib>     //exit
#include <cstring>     //strerror
#include <errno.h>     //errno
#include <iostream>    //cout
#include <signal.h>    //signal
#include <sys/prctl.h> // prctl
#include <unistd.h>    //sleep
using namespace std;
int main(int argc, char **argv)
{
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //��������Ϊ0���������
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�
    int pipe1[2], pipe2[2];
    int ret;
    ret = pipe(pipe1);
    if (ret == -1)
        ERR_EXIT("Error: Pipe1");
    ret = pipe(pipe2);
    if (ret == -1)
        ERR_EXIT("Error: Pipe2");
    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("Error: Fork");
    else if (pid == 0) //�ӽ���
    {
        prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
        //�ӽ������ܵ�1�ж����ݣ����ܵ�2��д����
        close(pipe1[1]);
        close(pipe2[0]);
        char buf[1024];
        int ret = read(pipe1[0], buf, sizeof(buf));
        if (ret == -1)
            ERR_EXIT("Error: Read");
        cout << "Child process(pid:" << getpid() << ") has read from parent process(pid:" << getppid() << ") ->" << buf << endl;

        ret = write(pipe2[1], buf, sizeof(buf));
        if (ret == -1)
            ERR_EXIT("Error: Write");
        cout << "Child process(pid:" << getpid() << ") has written to parent process(pid:" << getppid() << ") ->" << buf << endl;
        while (1)
            sleep(1);
        exit(EXIT_SUCCESS);
    }
    else //������
    {
        //���������ܵ�1��д���ݣ����ܵ�2�ж�����
        close(pipe1[0]);
        close(pipe2[1]);
        string send_str = "1951705 ������\n";
        ret = write(pipe1[1], send_str.c_str(), send_str.size());
        if (ret == -1)
            ERR_EXIT("Error: Write");
        cout << "Parent process(pid:" << getpid() << ") has written to child process(pid:" << pid << ") ->" << send_str << endl;

        char buf[1024];
        int ret = read(pipe2[0], buf, sizeof(buf));
        if (ret == -1)
            ERR_EXIT("Error: Read");
        cout << "Parent process(pid:" << getpid() << ") has read from child process(pid:" << pid << ") ->" << buf << endl;

        while (1)
            sleep(1);
    }
    return 0;
}
