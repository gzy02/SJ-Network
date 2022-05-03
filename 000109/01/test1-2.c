#include<stdio.h>
#include<unistd.h>
int main()
{
    setbuf(stdout,NULL);
    while(1)
    {
        printf("¸ßÔøÒê\n");
        sleep(5);
    }
    return 0;
}
