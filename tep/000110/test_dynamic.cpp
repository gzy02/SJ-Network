#include <stdio.h>
#include <stdlib.h>
#include <time.h>
char *test = NULL;
int getnum(const char *str)
{
    int s = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        s *= 10;
        s += str[i] - '0';
        i += 1;
    }
    return s;
}
int main(int argc, char **argv)
{
    int num = getnum(argv[1]);
    printf("%dMB\n", num);
    test = (char *)malloc(num * sizeof(char) * 1024 * 1024);
    if (test == NULL)
        perror("malloc error");
    return 0;
}
