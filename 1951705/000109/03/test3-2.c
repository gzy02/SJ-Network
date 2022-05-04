#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
void my_daemon(int nochdir, int noclose);
void ERR_EXIT(const char *m);

int main()
{
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    signal(SIGCHLD, SIG_IGN); // 75s
    setbuf(stdout, NULL);
    for (int i = 0; i < 10; ++i) // forkʮ���ӽ���
    {
        sleep(3);
        int pid = fork();
        int status = -1;
        if (pid == -1)
        {
            ERR_EXIT("fork error");
        }
        else if (pid == 0) //�ӽ���
        {
            for (int j = 0; j < 3; ++j) //��ӡ����
            {
                printf("%d %d 1951705 sub\n", getppid(), getpid());
                sleep(25);
            }
            exit(EXIT_SUCCESS);
        }
        // else
        //{
        //     wait(&status); //������ִ�е��ˣ����������Լ���ֱ�����ӽ��̽�����
        //���������ӽ��̽���ʱ���ͻ����������Դ�� 750s
        // }
    }
    while (1)
    {
        printf("%d %d 1951705 main\n", getppid(), getpid());
        sleep(5);
    }

    return 0;
}
