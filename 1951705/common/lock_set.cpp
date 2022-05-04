#include <cstdlib>             //exit
#include <cstring>             //strerror
#include <errno.h>             //errno
#include <fcntl.h>             //flock
#include <stdio.h>             //printf
#include <unistd.h>            //sleep
int lock_set(int fd, int type) //上锁/解锁
{
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_type = type;
    lock.l_pid = getpid();

    /* 判断文件锁的情况 */
    fcntl(fd, F_GETLK, &lock);
    if (lock.l_pid != getpid() || (lock.l_type != type && lock.l_type != F_UNLCK))
    {
        if (lock.l_type == F_RDLCK)
        {
            printf("Read lock already set by %d\n", lock.l_pid);
        }
        else if (lock.l_type == F_WRLCK)
        {
            printf("Write lock already set by %d\n", lock.l_pid);
        }
        sleep(1);
        return 0;
    }

    /* l_type 可能已被F_GETLK修改过 */
    lock.l_type = type;
    /* 根据不同的type值进行阻塞式上锁或解锁 */
    if ((fcntl(fd, F_SETLK, &lock)) < 0)
    {
        printf("Lock failed:type = %d\n", lock.l_type);
        exit(-1);
    }

    switch (lock.l_type)
    {
    case F_RDLCK:
        printf("Read lock set by %d\n", getpid());
        break;
    case F_WRLCK:
        printf("Write lock set by %d\n", getpid());
        break;
    case F_UNLCK:
        printf("Release lock by %d\n", getpid());
        break;
    } /* end of switch */
    return 1;
}
