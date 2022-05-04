#include "../../include/my_optarg.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main(int argc, char **argv)
{
    my_daemon(0, 0);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //���û�����Ϊ�գ�ֱ�����
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�

    Myoptarg arg;
    Set_Config(argc, argv, &arg);
    int fork_error = 0, malloc_error = 0;
    for (int i = 1; i <= arg.fnum; ++i) // fork fnum���ӽ���
    {
        if (fork_error || malloc_error)
            break;
        int pid = fork();
        if (pid == -1)
        {
            printf("fork error");
            fork_error = 1;
        }
        else if (pid == 0) //�ӽ���
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
            char *str = (char *)malloc(arg.size * 1024 * sizeof(char));
            if (str == NULL)
            {
                printf("malloc error");
                malloc_error = 1;
            }
            else if (arg.init == 1)
            {
                srand((unsigned)time(NULL));
                for (int k = 0; k < arg.size * 1024; ++k)
                    str[k] = rand() % 256;
            }
            while (1)
                sleep(1);
        }
        else //������
        {
            if (i % arg.pnum == 0)
                printf("\n�ѷ���%d���ӽ���", i);
        }
    }
    if (fork_error == 1)
        printf("fork error");
    if (malloc_error == 1)
        printf("malloc error");
    while (1)
        sleep(1); //������ò��Ҫsleep
    return 0;
}
// ./test6 --fnum 1000 --size 1024 --init yes --pnum 100
// ./test6 --fnum 1000 --size 5120 --init yes --pnum 100
// ./test6 --fnum 1000 --size 1024 --init no --pnum 100
// ./test6 --fnum 1000 --size 5120 --init no --pnum 100
