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
    printf("��ǰ����=%d�ֽ�\n�������=%d�ֽ�\n", limit.rlim_cur, limit.rlim_max);

    int num = digit(argv[1]);
    char *test = NULL;
    test = (char *)malloc(size * num);
    if (test == NULL)
    {
        printf("�ڴ�Ҫ�󳬹�������,����������������Ӳ����\n");
        limit.rlim_cur = limit.rlim_max;
        int a = setrlimit(RLIMIT_AS, &limit);

        if (0 == a)
        {
            printf("�������޸ĳɹ�\n");
            test = (char *)malloc(size * num);
            if (test == NULL)
            {
                printf("��������Ӳ����\n");
                limit.rlim_max *= 2;
                limit.rlim_cur *= 2;
                a = setrlimit(RLIMIT_AS, &limit);

                if (a == -1)
                {
                    printf("�޸�ʧ��\n");
                }
            }
        }
    }
    memset(test, 0, size * num);
    free(test);
    return 0;
}
