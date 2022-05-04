#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#define size sizeof(char) * 1024 * 1024
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
    struct rlimit limit;
    getrlimit(RLIMIT_AS, &limit);
    printf("当前限制=%d字节\n最大限制=%d字节\n", limit.rlim_cur, limit.rlim_max);

    int num = digit(argv[1]);
    char *test = NULL;
    test = (char *)malloc(size * num);
    if (test == NULL)
    {
        printf("内存要求超过软限制,尝试提升软限制至硬限制\n");
        limit.rlim_cur = limit.rlim_max;
        int a = setrlimit(RLIMIT_AS, &limit);

        if (0 == a)
        {
            printf("软限制修改成功\n");
            test = (char *)malloc(size * num);
            if (test == NULL)
            {
                printf("尝试提升硬限制\n");
                limit.rlim_max *= 2;
                limit.rlim_cur *= 2;
                a = setrlimit(RLIMIT_AS, &limit);

                if (a == -1)
                {
                    printf("修改失败\n");
                }
            }
        }
    }
    memset(test, 0, size * num);
    free(test);
    return 0;
}
