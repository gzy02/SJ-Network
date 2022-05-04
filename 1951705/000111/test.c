#include "readconfig.c"
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define NAME_LENGTH 128 //��������󳤶�

void handle(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) //���������ӽ���
        ;
}

int main(int argc, char **argv, char **env)
{
    my_daemon_with_console(0, 1); //������̨��α�ػ�����
    char value[MAXN];
    read_dat("/home/stu/u1951705/1951705.dat", value);
    printf("%s", value);
    int num;
    num = read_conf("/etc/stu/stu_1951705.conf");
    // printf("�ӽ�������=%d\n", num);

    int sec, hour, min, all;
    time_t start = time(NULL), now; //�����̿�ʼ��ʱ

    signal(SIGCHLD, handle);

    int *flags = (int *)malloc(sizeof(int) * num); //�洢�ӽ���pid��
    if (flags == NULL)
    {
        perror("malloc error\n");
        exit(-1);
    }
    for (int i = 0; i < num; ++i)
    {
        flags[i] = 0;
    }

    char newfullname[NAME_LENGTH];
    char childname[NAME_LENGTH];
    char originname[NAME_LENGTH];
    strcpy(originname, argv[0]);    //����argv[0]
    char tepshortname[NAME_LENGTH]; //��ʱ�����������ڴ洢������

    char newshortname[NAME_LENGTH];
    memset(tepshortname, 0, sizeof(tepshortname));
    sprintf(newshortname, strcat(strcpy(tepshortname, originname), " [main]"));
    prctl(PR_SET_NAME, newshortname); // psָ�Shell��������ҵ���Ľ�������

    while (1)
    {
        for (int i = 0; i < num; ++i)
        {
            sprintf(childname, "/proc/%d", flags[i]);
            if (flags[i] == 0 || access(childname, 0) == -1) // flagsδ��ʼ�� ���ӽ��̲�����(�����ܲ��ܷ���/proc/{flags[i]}����֪��)
            {
                pid_t pid;
                while ((pid = fork()) == -1)
                {
                    perror("Fork error.\n");
                    sleep(1);
                }
                if (pid == 0)
                {
                    // prctl(PR_SET_PDEATHSIG, SIGKILL); //�ӽ��̽��ܸ����̵������źű�ɱ��
                    //����pdf����д�˲���ɱ�ӽ��̣�ֻ������辶getppid()�ж��Լ��Ƿ��ǹ¶�����
                    start = time(NULL); //���¼���
                    memset(tepshortname, 0, sizeof(tepshortname));
                    sprintf(newshortname, strcat(strcpy(tepshortname, originname), " [Sub-%02d]"), i + 1);
                    prctl(PR_SET_NAME, newshortname);

                    while (1)
                    {
                        if (getppid() == 1) //�����Լ��ǹ¶����̣��Լ��˳��ˣ�ȷʵû��ɱ�ӽ���
                        {
                            exit(0);
                        }
                        now = time(NULL);
                        all = now - start;
                        sec = all % 60;
                        min = (all / 60) % 60;
                        hour = all / 3600;
                        memset(tepshortname, 0, sizeof(tepshortname));
                        sprintf(newfullname, strcat(strcpy(tepshortname, originname), " [Sub-%02d %02d:%02d:%02d]"), i + 1, hour, min, sec);
                        strcpy(argv[0], newfullname); // ps -ef�Ľ������޸�

                        sleep(1); //��ѭ������sleep
                    }
                    exit(0);
                }
                else //������
                {
                    flags[i] = pid;
                    break;
                }
            }
        }

        //������
        now = time(NULL);
        all = now - start;
        sec = all % 60;
        min = (all / 60) % 60;
        hour = all / 3600;
        memset(tepshortname, 0, sizeof(tepshortname));
        sprintf(newfullname, strcat(strcpy(tepshortname, originname), " [main %02d:%02d:%02d]"), hour, min, sec);
        strcpy(argv[0], newfullname);

        sleep(1); //��ѭ������sleep
    }
    free(flags);
    return 0;
}
