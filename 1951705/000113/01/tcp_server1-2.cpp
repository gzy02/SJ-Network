#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/my_daemon.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>     //exit
#include <cstring>     //strerror
#include <errno.h>     //error
#include <iostream>    //cout
#include <signal.h>    //signal
#include <sys/prctl.h> // prctl
#include <unistd.h>    //fork
using namespace std;

int main(int argc, char **argv)
{
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //��������Ϊ0���������
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); //
    cur_connect.Setreadbytes(20);             //����Ϊ20�ֽ�
    while (true)
    {
        int cur_fd = cur_connect.AcceptClient(); //��Client����
        if (cur_fd == -1)
        {
            cout << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        int pid = fork();
        if (pid == -1)
            ERR_EXIT("Error: Fork");
        else if (pid == 0) //�ӽ���
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
            ssize_t len = cur_connect.Read(cur_fd);
            cout << "len: " << len << endl;

            cout << errno << endl
                 << strerror(errno) << endl;

            cout << "Recv_str:" << cur_connect.Getread_str() << endl;
            exit(EXIT_SUCCESS);
        }
        else //�����̽�������ѭ��������accept
            continue;
    }
    return 0;
}
