#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void my_daemon_with_console(int nochdir, int noclose)
{
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) //������
        exit(EXIT_SUCCESS);
    /*
    //ע����һ���ּ�����ʹ�ý��̳�Ϊ�½�������鳤�͸ûỰ���׽��̣�
    //���ɲ��������̨
    if(setsid() == -1)
    {
        perror("SETSID ERROR");
        exit(EXIT_FAILURE);
    }
    */
    if (nochdir == 0)
        chdir("/");
    if (noclose == 0)
    {
        int i;
        for (i = 0; i < 3; ++i)
        {
            close(i);
            open("/dev/null", O_RDWR);
        }
    }
    umask(0);
    return;
}

int read_conf(const char *path)
{
    FILE *fd = NULL;
    fd = fopen(path, "r");
    if (fd == NULL)
    {
        perror("Conf open error.\nNo such file.\n");
        exit(-1);
    }
    int a;
    if (fscanf(fd, "�ӽ�������=%d", &a) != 1)
    {
        perror("Conf read error.\n");
        exit(-1);
    }
    fclose(fd);
    return a;
}

#define MAXN 1024
void read_dat(const char *path, char *value)
{
    FILE *fd = NULL;
    fd = fopen(path, "r");
    if (fd == NULL)
    {
        perror("Data open error.\nNo such file.\n");
        exit(-1);
    }
    char pre[MAXN], *check;
    while (1)
    {
        check = fgets(pre, MAXN, fd);
        if (check == NULL)
        {
            if (ferror(fd))
            {
                perror("Data read error.\n");
                exit(-1);
            }
        }
        else
        {
            strcpy(value, pre);
            // printf("%s\n", pre); //��ӡ
        }
        if (feof(fd)) //�ļ�β
        {
            break;
        }
    }
    fclose(fd);
}
