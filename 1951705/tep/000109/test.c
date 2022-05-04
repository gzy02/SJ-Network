#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    /*2. 获取当前系统内存使用情况*/
    struct sysinfo s_info;
    char info_buff[100];
    while (1)
    {
        if (sysinfo(&s_info) == 0)
        {
            sprintf(info_buff, "总内存: %.ld M", s_info.totalram / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "未使用内存: %.ld M", s_info.freeram / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "交换区总内存: %.ld M", s_info.totalswap / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "交换区未使用内存: %.ld M", s_info.freeswap / 1024 / 1024);
            printf("%s\n", info_buff);

            sprintf(info_buff, "系统运行时间: %.ld 分钟", s_info.uptime / 60);
            printf("%s\n", info_buff);

            printf("\n\n");
        }
        sleep(1);
    }
    return 0;
}
