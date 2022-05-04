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
    my_daemon(1, 1); //Ŀ¼��Ȼ�Ǳ�Ŀ¼�������ļ�������

    int shm_id = -1;           //���干���ڴ��
    char *shared_memory;       //���干���ڴ�ָ��
    struct shmid_ds shmbuffer; //���干���ڴ滺��
    size_t shm_size = 0x6400;  //���干���ڴ��С
    key_t mykey = 1234;        // key
    while (shm_id == -1)
    {
        shm_id = shmget(mykey, shm_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); //�����������ڴ���
        sleep(1);
    }
    shared_memory = (char *)shmat(shm_id, 0, 0); //�󶨵������ڴ�
    printf("shared memory attached at address %p\n", shared_memory);
    memset(shared_memory, 0, shm_size);

    while (1)
    {
        if (strlen(shared_memory) != 0)
        {
            printf("test5-2-2 process received:\n%s\n", shared_memory);
            const char buf[] = "1951705 ������ test5-2-2\n";
            sprintf(shared_memory, strcat(shared_memory, buf));
            printf("test5-2-2 process wrote:\n%s\n", buf);
            break;
        }
        sleep(1);
    }

    return 0;
}
