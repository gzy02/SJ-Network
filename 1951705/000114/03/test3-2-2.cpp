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
    signal(MY_SIG_47, show);
    signal(MY_SIG_53, end);
    while (1)
        sleep(1);
    return 0;
}
