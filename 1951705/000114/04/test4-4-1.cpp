#ifdef _GNU_SOURCE
#undef _GNU_SOURCE
#endif
#include "../../include/my_daemon.h"
#include <cstdlib>     //exit
#include <cstring>     //strerror
#include <errno.h>     //errno
#include <iostream>    //cout
#include <signal.h>    //signal
#include <sys/msg.h>   //msgbuf
#include <sys/prctl.h> // prctl
#include <unistd.h>    //sleep
using namespace std;
struct msgbuf
{
    long mtype;      /* message type, must be > 0 */
    char mtext[128]; /* message data */
};
int main(int argc, char **argv)
{
    my_daemon(1, 1); //Ŀ¼��Ȼ�Ǳ�Ŀ¼�������ļ�������

    key_t key = ftok("/", 0);

    int msgid = msgget(key, IPC_CREAT | 0666);
    cout << "The key is " << key << endl;
    cout << "The msgid is " << msgid << endl;
    if (msgid == -1)
        ERR_EXIT("Error: Msgget");
    msgbuf data;
    data.mtype = 1;
    strcpy(data.mtext, "1951705 ������");
    int ret = msgsnd(msgid, &data, sizeof(data), 0);
    if (ret == -1)
        ERR_EXIT("Error: Msgsnd");
    cout << "SendStr:" << data.mtext << endl;
    while (1)
        sleep(1);

    return 0;
}
