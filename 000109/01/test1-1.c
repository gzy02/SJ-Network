#include<stdio.h>
#include<unistd.h>
int main()
{
    setbuf(stdout,NULL);
    while(1)
    {
        printf("1951705\n");
        sleep(5);
    }
    return 0;
}
