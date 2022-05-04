#include "../../include/my_optarg.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main(int argc, char **argv)
{
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //���û�����Ϊ�գ�ֱ�����
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�

    Myoptarg arg;
    Set_Config(argc, argv, &arg);

    for (int i = 1; i <= arg.fnum; ++i) // fork fnum���ӽ���
    {
        int pid = fork();
        if (pid == -1)
        {
            ERR_EXIT("fork error");
        }
        else if (pid == 0) //�ӽ���
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
            char *str = (char *)malloc(arg.size * 1024 * sizeof(char));
            while (1)
            {
                ;
            }
        }
        else //������
        {
            if (i % arg.pnum == 0)
                printf("�ѷ���%d���ӽ���", i);
        }
    }
    while (1)
    {
        ;
    }

    return 0;
}
