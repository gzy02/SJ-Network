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

    int shm_id;                //���干���ڴ��
    char *shared_memory;       //���干���ڴ�ָ��
    struct shmid_ds shmbuffer; //���干���ڴ滺��
    int shm_size;              //���干���ڴ��С

    shm_id = shmget(IPC_PRIVATE, 0x6400, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); //����һ�������ڴ���
    shared_memory = (char *)shmat(shm_id, 0, 0);                                    //�󶨵������ڴ�
    printf("shared memory attached at address %p\n", shared_memory);

    shmctl(shm_id, IPC_STAT, &shmbuffer); //�������ڴ�ṹstruct shmid_ds
    shm_size = shmbuffer.shm_segsz;       //�Խṹstruct shmid_ds��ȡ�ڴ��С
    printf("segment size:%d\n", shm_size);

    shmdt(shared_memory);                                       //����ù����ڴ�
    shared_memory = (char *)shmat(shm_id, (void *)0x500000, 0); //���°󶨹����ڴ�
    printf("shared memory reattched at address %p\n", shared_memory);
    memset(shared_memory, 0, shm_size);

    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("Error: Fork");
    else if (pid == 0) //�ӽ���
    {
        while (1)
        {
            if (strlen(shared_memory) != 0)
            {
                printf("Child process received:\n%s\n", shared_memory);
                const char buf[] = "1951705 ������ Child\n";
                sprintf(shared_memory, strcat(shared_memory, buf));
                printf("Child process wrote:\n%s\n", buf);
                break;
            }
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    }
    else //������
    {
        const char buf[] = "1951705 ������ Parent\n";
        const int size = strlen(buf);
        sprintf(shared_memory, buf); //�����ڴ���д��һ���ַ���
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

        shmdt(shared_memory);        //����ù����ڴ�
        shmctl(shm_id, IPC_RMID, 0); //�ͷŹ����ڴ�
    }

    return 0;
}
