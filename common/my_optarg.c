#include "../include/my_optarg.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *pattern[PATTERN_SIZE] = {"fnum", "size", "pnum", "init", "ptime"};

int digit(int num)
{
    int start = 1;
    while (num / 10)
    {
        start++;
        num /= 10;
    }
    return start;
}
int StrToInt(const char *str)
{
    int flags = (str[0] == '-');
    int abs = 2 * flags - 1;
    int result;
    sscanf(str, "%d", &result);
    if (!(result * abs < 0 && strlen(str + flags) == digit(result)))
    {
        result = flags ? INT_MIN : INT_MAX;
    }
    return result;
}
void Init_Config(struct Myoptarg *arg)
{
    arg->fnum = arg->size = arg->pnum = arg->ptime = arg->init = 0;
}
void Print_Config(struct Myoptarg *arg)
{
    printf("fnum=%d,size=%d,pnum=%d,ptime=%d,init=%d\n", arg->fnum, arg->size, arg->pnum, arg->ptime, arg->init);
}
void Set_Config(int argc, char **argv, struct Myoptarg *arg)
{
    Init_Config(arg);

    for (int i = 1; i < argc; ++i)
    {
        // printf("%s", &argv[1][2]);
        if (strlen(argv[i]) >= 2 && argv[1][0] == '-' && argv[1][1] == '-') //²ÎÊý
        {
            // printf("%s", &argv[i][2]);
            if (!strcmp(&argv[i][2], "fnum"))
            {
                if (i + 1 < argc)
                    arg->fnum = StrToInt(argv[i + 1]);
            }
            else if (!strcmp(&argv[i][2], "size"))
            {
                if (i + 1 < argc)
                    arg->size = StrToInt(argv[i + 1]);
            }
            else if (!strcmp(&argv[i][2], "pnum"))
            {
                if (i + 1 < argc)
                    arg->pnum = StrToInt(argv[i + 1]);
            }
            else if (!strcmp(&argv[i][2], "init"))
            {
                if (i + 1 < argc)
                    arg->init = !strcmp(argv[i + 1], "yes");
            }
            else if (!strcmp(&argv[i][2], "ptime"))
            {
                if (i + 1 < argc)
                    arg->ptime = StrToInt(argv[i + 1]);
            }
        }
    }
}
