#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    int num = digit(argv[1]);
    int type = digit(argv[2]); // open系统调用还是fopen
    int clos = digit(argv[3]);
    int fd, check;
    FILE *file = NULL;
    char name[40], result[100];
    struct rlimit limit;
    getrlimit(RLIMIT_NOFILE, &limit);
    printf("当前限制=%d个文件\n最大限制=%d个文件\n", limit.rlim_cur, limit.rlim_max);

    setbuf(stdout, NULL);
    if (clos) //是否关闭三个标准输入输出文件,这里只关stdin，stderr
    {
        close(0);
        close(2);
    }
    for (int i = 0; i < num; i++)
    {
        sprintf(name, "./test/%d.txt", i);
        switch (type)
        {
        case 1:
            fd = open(name, O_CREAT | O_RDWR);
            check = fd >= 0;
            break;
        case 2:
            file = fopen(name, "w+");
            check = file != NULL;
            break;
        }
        if (check)
        {
            sprintf(result, "open %s success\n", name);
        }
        else
        {
            sprintf(result, "open %s fail\n", name);
        }
        printf("%s", result);
    }
    while (1)
        ;
    return 0;
}
