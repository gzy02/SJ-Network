#include "../include/local_ip_mac_info.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
// get the ip address by domain
int get_ip_by_domain(const char *domain, char *ip) // 根据域名获取ip
{
    char **pptr;
    struct hostent *hptr;

    hptr = gethostbyname(domain);
    if (NULL == hptr)
    {
        printf("Gethostbyname error for host:%s/n", domain);
        return -1;
    }

    for (pptr = hptr->h_addr_list; *pptr != NULL; pptr++)
    {
        if (NULL != inet_ntop(hptr->h_addrtype, *pptr, ip, IP_SIZE))
        {
            return 0; // 只获取第一个 ip
        }
    }
    return -1;
}
