#include "../../include/my_daemon.h"
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int main(void)
{
    my_daemon(1, 1); //目录依然是本目录，不关文件描述符

    int shm_id = -1;           //定义共享内存键
    char *shared_memory;       //定义共享内存指针
    struct shmid_ds shmbuffer; //定义共享内存缓冲
    size_t shm_size = 0x6400;  //定义共享内存大小
    key_t mykey = 1234;        // key
    while (shm_id == -1)
    {
        shm_id = shmget(mykey, shm_size, IPC_EXCL | S_IRUSR | S_IWUSR); //不允许创建，只允许连接到一个已创建的共享内存区
        sleep(1);
    }

    shared_memory = (char *)shmat(shm_id, 0, 0); //绑定到共享内存
    printf("shared memory attached at address %p\n", shared_memory);

    const char buf[] = "1951705 高曾谊 test5-2-1\n";
    const int size = strlen(buf);
    sprintf(shared_memory, buf); //向共享内存中写入一个字符串
    printf("test5-2-1 process wrote:\n%s\n", shared_memory);
    while (1)
    {
        if (strlen(shared_memory) != size)
        {
            printf("test5-2-1 process received:\n%s\n", shared_memory);
            break;
        }
        sleep(1);
    }

    shmdt(shared_memory);        //脱离该共享内存
    shmctl(shm_id, IPC_RMID, 0); //释放共享内存

    return 0;
}
