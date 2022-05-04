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

#define MY_SIG_47 SIGRTMIN + 13
#define MY_SIG_53 SIGRTMAX - 11

void handle(int num)
{
    printf("�յ��ӽ����źţ��������˳�\n");
    exit(EXIT_SUCCESS);
}
void show(int num)
{
    printf("pid:%d get %d\n", getpid(), num);
}
void end(int num)
{
    printf("pid:%d get %d,exit\n", getpid(), num);
    exit(EXIT_SUCCESS);
}
int main()
{
    my_daemon(1, 1);
    pid_t pid;
    signal(SIGCHLD, handle); //�ӽ����˳�ʱ������ִ��handle
    signal(MY_SIG_47, show);
    signal(MY_SIG_53, end);
    pid = fork();
    if (pid < 0)
        ERR_EXIT("Error: Fork");

    else if (pid == 0) //�ӽ���
    {
        while (1)
            sleep(1);
        exit(EXIT_SUCCESS);
    }
    else //������
    {
        while (1)
        {
            kill(pid, MY_SIG_47); //���ӽ��̷�47
            sleep(1);
        }
    }
    return 0;
}
