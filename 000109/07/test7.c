#include "../../include/my_optarg.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int cnt_split = 0, cnt_recovery = 0; //ȫ�ּ�����

void handle(int pid)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) //Ҫд-1����ʾ�����ӽ��̣�дpid�Ļ����ܵ���һЩ���⣨��Ƭ�ӽ������٣���Щû��ʱ���ճ�Ϊ��ʬ���̣�
    {
        cnt_recovery += 1;
    }
}
int main(int argc, char **argv)
{
    my_daemon(0, 1);      //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL); //���û�����Ϊ�գ�ֱ�����
    // signal(SIGCHLD, SIG_IGN);
    signal(SIGCHLD, handle); //�ӽ���״̬�ı�ʱҪ��������һ���ź�

    struct rlimit nproc_limit;
    getrlimit(RLIMIT_NPROC, &nproc_limit);
    printf("\n��ǰ���ƣ�%d������\n������ƣ�%d������\n", nproc_limit.rlim_cur, nproc_limit.rlim_max);

    struct sysinfo s_info;

    time_t now, start;
    start = time(NULL);
    Myoptarg arg;
    Set_Config(argc, argv, &arg);
    int bias = 20;                  //Ԥ�����Ľ��̸�������ֹ�ﵽ������
    for (int i = 1; i <= arg.fnum;) // fork fnum���ӽ���
    {
        now = time(NULL);
        if ((now - start) >= arg.ptime)
        {
            start = now;
            printf("�ѷ���%d���ӽ���    �ѻ���%d���ӽ���\n", cnt_split, cnt_recovery);
        }
        if (sysinfo(&s_info) == 0)
        {
            // printf("%d\n", s_info.freeram / 1024 / 1024);
            if (s_info.freeram / 1024 > arg.size * sizeof(char) * 5 && cnt_split - cnt_recovery < nproc_limit.rlim_cur - bias) // ʣ���ڴ泬��arg.size * sizeof(char)���屶��fork
            {
                int pid = fork();
                if (pid == -1)
                {
                    perror("Fork error");
                    {
                        printf("�û�ӵ�еĽ�����������������,�˴�forkȡ��\n��ϵͳ����ʱ������ִ�д˴�fork\n");
                        --i; //����fork�������
                        --cnt_split;
                        bias += nproc_limit.rlim_cur - (cnt_split - cnt_recovery); // bias��������֮��Ĳ��
                    }
                }
                else if (pid == 0) //�ӽ���
                {
                    prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
                    char *str = (char *)malloc(arg.size * 1024 * sizeof(char));
                    if (str == NULL)
                    {
                        ERR_EXIT("malloc error");
                    }
                    //������������ֵ
                    {
                        srand((unsigned)time(NULL));
                        for (int k = 0; k < arg.size * 1024; ++k)
                            str[k] = rand() % 256;
                    }
                    sleep(1); // 20
                    exit(0);
                }
                else //������
                {
                    ++cnt_split;
                    ++i;
                }
            }
        }
    }

    while (1)
    {
        printf("�ѷ���%d���ӽ���    �ѻ���%d���ӽ���\n", cnt_split, cnt_recovery);
        sleep(arg.ptime); //Ҫsleep
    }
    return 0;
}
// ./test7 --fnum 50000 --size 1024 --ptime 10
