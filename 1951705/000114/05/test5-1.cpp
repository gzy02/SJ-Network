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

    int shm_id;                //定义共享内存键
    char *shared_memory;       //定义共享内存指针
    struct shmid_ds shmbuffer; //定义共享内存缓冲
    int shm_size;              //定义共享内存大小

    shm_id = shmget(IPC_PRIVATE, 0x6400, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); //创建一个共享内存区
    shared_memory = (char *)shmat(shm_id, 0, 0);                                    //绑定到共享内存
    printf("shared memory attached at address %p\n", shared_memory);

    shmctl(shm_id, IPC_STAT, &shmbuffer); //读共享内存结构struct shmid_ds
    shm_size = shmbuffer.shm_segsz;       //自结构struct shmid_ds获取内存大小
    printf("segment size:%d\n", shm_size);

    shmdt(shared_memory);                                       //脱离该共享内存
    shared_memory = (char *)shmat(shm_id, (void *)0x500000, 0); //重新绑定共享内存
    printf("shared memory reattched at address %p\n", shared_memory);
    memset(shared_memory, 0, shm_size);

    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("Error: Fork");
    else if (pid == 0) //子进程
    {
        while (1)
        {
            if (strlen(shared_memory) != 0)
            {
                printf("Child process received:\n%s\n", shared_memory);
                const char buf[] = "1951705 高曾谊 Child\n";
                sprintf(shared_memory, strcat(shared_memory, buf));
                printf("Child process wrote:\n%s\n", buf);
                break;
            }
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    }
    else //父进程
    {
        const char buf[] = "1951705 高曾谊 Parent\n";
        const int size = strlen(buf);
        sprintf(shared_memory, buf); //向共享内存中写入一个字符串
        printf("Parent process wrote:\n%s\n", shared_memory);
        while (1)
        {
            if (strlen(shared_memory) != size)
            {
                printf("Parent process received:\n%s\n", shared_memory);
                break;
            }
            sleep(1);
        }

        shmdt(shared_memory);        //脱离该共享内存
        shmctl(shm_id, IPC_RMID, 0); //释放共享内存
    }

    return 0;
}
