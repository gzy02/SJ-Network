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

int main()
{
    my_daemon(1, 1);
    char buf[100];
    int cnt = 9;
    FILE *fp;
    while (1)
    {
        fp = popen("pidof test3-2-2", "r");
        fgets(buf, sizeof(buf), fp);
        pid_t pid = atoi(buf);
        if (pid != 0) //ÃΩ≤‚µΩ¡À
        {
            if (cnt != 0)
                kill(pid, MY_SIG_47);
            if (cnt == 0)
            {
                kill(pid, MY_SIG_53);
                break;
            }
            cnt--;
        }
        sleep(1);
    }

    return 0;
}
