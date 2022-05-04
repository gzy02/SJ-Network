#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/my_daemon.h"
#include "../../include/socket_optarg.h"
#include <climits>
#include <cstdlib>      //exit
#include <cstring>      //strerror
#include <errno.h>      //error
#include <iostream>     //cout
#include <signal.h>     //signal
#include <sys/prctl.h>  // prctl
#include <sys/select.h> //select
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int main(int argc, char **argv)
{
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //��������Ϊ0���������
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); // listen(fd,1)
    fd_set rset, allset;
    int max_fd = INT_MIN;
    while (1)
    {
        int cur_fd = cur_connect.AcceptClient(); //��Client����
        cur_connect.Setaccept_fd_nonblock(cur_fd);
        FD_ZERO(&allset); //ֻ����һ��
        FD_SET(cur_fd, &allset);
        rset = allset;
        max_fd = max(max_fd, cur_fd);

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
            int ready_num = select(cur_fd + 1, &rset, NULL, NULL, NULL);
            cout << "ready_num: " << ready_num << endl;
            cout << errno << endl
                 << strerror(errno) << endl;
            ssize_t len = cur_connect.Read(cur_fd);
            cout << "len: " << len << endl;
            cout << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_SUCCESS);
        }
        else //�����̽�������ѭ��������accept
            continue;
    }

    return 0;
}
