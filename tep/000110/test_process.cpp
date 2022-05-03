#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <unistd.h>
int digit(const char *str)
{
    int s = 0, i = 0;
    while (str[i])
    {
        s *= 10;
        s += str[i] - '0';
        i += 1;
    }
    return s;
}
int main(int argc, char **argv)
{
    setbuf(stdout, NULL);     //���û�����Ϊ�գ�ֱ�����
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�
    struct rlimit limit;
    getrlimit(RLIMIT_NPROC, &limit);
    printf("��ǰ����=%d������\n�������=%d������\n", limit.rlim_cur, limit.rlim_max);
    int num = digit(argv[1]);
    for (int i = 1; i <= num; ++i)
    {
        int pid = fork();
        if (pid == -1)
        {
            printf("fork error\n");
            if (limit.rlim_cur < limit.rlim_max)
            {
                printf("�û�ӵ�еĽ�����������������,����������������Ӳ����\n");
                limit.rlim_cur = limit.rlim_max;
                int tep = setrlimit(RLIMIT_NPROC, &limit);
                if (0 == tep)
                {
                    printf("�������޸ĳɹ�\n");
                    printf("��ǰ����=%d������\n�������=%d������\n", limit.rlim_cur, limit.rlim_max);
                    --i; //����fork
                }
                else
                {
                    printf("�������޸�ʧ��\n");
                }
            }
            else
            {
                printf("��������Ӳ����\n");
                limit.rlim_max *= 2;
                limit.rlim_cur *= 2;
                int tep = setrlimit(RLIMIT_NPROC, &limit);
                if (0 == tep)
                {
                    printf("Ӳ�����޸ĳɹ�\n");
                    printf("��ǰ����=%d������\n�������=%d������\n", limit.rlim_cur, limit.rlim_max);
                    --i; //����fork
                }
                else
                {
                    printf("Ӳ�����޸�ʧ��\n");
                }
            }
        }
        else if (pid == 0)
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
            sleep(-1);
        }
        else
        {
            printf("�ѷ���%d���ӽ���\n", i);
        }
    }
    sleep(-1);
    return 0;
}
