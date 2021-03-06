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
    my_daemon(1, 1); //目录依然是本目录，不关文件描述符
    key_t key = ftok(".", 0);

    int msgid = msgget(key, IPC_CREAT | 0666);
    cout << "The key is " << key << endl;
    cout << "The msgid is " << msgid << endl;
    if (msgid == -1)
        ERR_EXIT("Error: Msgget");
    msgbuf data;
    data.mtype = 1;
    strcpy(data.mtext, "1951705 高曾谊 Parent");
    int ret = msgsnd(msgid, &data, sizeof(data), 0);
    if (ret == -1)
        ERR_EXIT("Error: Msgsnd");
    cout << "SendStr:" << data.mtext << endl;

    ret = msgrcv(msgid, &data, sizeof(data), 2, 0); //这里是2
    if (ret < 0)
        ERR_EXIT("Error: Msgrcv");
    cout << "RecvBytes:" << ret << endl;
    cout << "RecvStr:" << data.mtext << endl;

    msgctl(msgid, IPC_RMID, NULL); //最后一个收的销毁msgid为标识符的消息队列
    while (1)
        sleep(1);

    return 0;
}
