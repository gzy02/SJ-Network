#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    /*2. ��ȡ��ǰϵͳ�ڴ�ʹ�����*/
    struct sysinfo s_info;
    char info_buff[100];
    while (1)
    {
        if (sysinfo(&s_info) == 0)
        {
            sprintf(info_buff, "���ڴ�: %.ld M", s_info.totalram / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "δʹ���ڴ�: %.ld M", s_info.freeram / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "���������ڴ�: %.ld M", s_info.totalswap / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "������δʹ���ڴ�: %.ld M", s_info.freeswap / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "ϵͳ����ʱ��: %.ld ����", s_info.uptime / 60);
            printf("%s\n", info_buff);

            printf("\n\n");
        }
        sleep(1);
    }
    return 0;
}
