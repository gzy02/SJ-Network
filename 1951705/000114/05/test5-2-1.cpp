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
        shm_id = shmget(mykey, shm_size, IPC_EXCL | S_IRUSR | S_IWUSR); //����������ֻ�������ӵ�һ���Ѵ����Ĺ����ڴ���
        sleep(1);
    }

    shared_memory = (char *)shmat(shm_id, 0, 0); //�󶨵������ڴ�
    printf("shared memory attached at address %p\n", shared_memory);

    const char buf[] = "1951705 ������ test5-2-1\n";
    const int size = strlen(buf);
    sprintf(shared_memory, buf); //�����ڴ���д��һ���ַ���
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

    shmdt(shared_memory);        //����ù����ڴ�
    shmctl(shm_id, IPC_RMID, 0); //�ͷŹ����ڴ�

    return 0;
}
