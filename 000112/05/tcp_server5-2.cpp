#include "../../include/local_ip_mac_info.h"
#include "../../include/socket_optarg.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int check(string arg_ip)
{
    char ip[IP_SIZE];
    // get all the information of interface
    int sockfd;
    struct ifconf ifconf;
    struct ifreq *ifreq;
    char buf[512];

    ifconf.ifc_len = 512; //默认值，可以被ioctl更改
    ifconf.ifc_buf = buf;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cerr << "Error: socket" << endl;
        exit(EXIT_FAILURE);
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);

    ifreq = (struct ifreq *)ifconf.ifc_buf;
    for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; i--)
    {
        if (ifreq->ifr_flags == AF_INET)
        {
            get_local_ip(ifreq->ifr_name, ip);
            cout << "name:" << ifreq->ifr_name << ",IP:" << ip << endl;
            if (strcmp(ip, arg_ip.c_str()) == 0)
                return 0;
            ifreq++;
        }
    }
    if (arg_ip == "0.0.0.0")
        return 0;
    else
        return -1;
}
int main(int argc, char **argv)
{
    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    if (arg.port == ARG_PORT_DEFAULT)
    {
        cerr << "Error: Please use --port [0,65535]" << endl;
        exit(EXIT_FAILURE);
    }
    if (arg.ip.size() == 0) // ip长度默认为0, 默认绑定所有
    {
        arg.ip = "0.0.0.0";
    }
    if (check(arg.ip) == -1) //检查过后发现不在其中
    {
        cerr << "Error: IP " << arg.ip << " not found" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "This is server" << endl;

    // socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        cerr << "Error: socket" << endl;
        exit(EXIT_FAILURE);
    }

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(arg.port);
    addr.sin_addr.s_addr = inet_addr(arg.ip.c_str()); //指定对应的网卡的地址，是一个unsigned int

    int opt = 1; // closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    // int rcvlowat = 20; //累计读20个字节才不阻塞
    // setsockopt(listenfd, SOL_SOCKET, SO_RCVLOWAT, &rcvlowat, sizeof(int));

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cerr << "Error: bind" << endl;
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(listenfd, 1) == -1)
    {
        cerr << "Error: listen" << endl;
        exit(EXIT_FAILURE);
    }

    // accept
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    cout << "listening..." << endl;
    conn = accept(listenfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (conn < 0)
    {
        cerr << "Error: accept" << endl;
        exit(EXIT_FAILURE);
    }

    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    cout << "connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << endl;

    const int read_size = 20;
    char buf[read_size + 1];
    memset(buf, 0, sizeof(buf));
    int len = recv(conn, buf, read_size, MSG_WAITALL); // 20个字节
    buf[len] = '\0';

    cout << "length:" << len << endl;
    cout << "message:" << buf << endl;

    close(conn);
    close(listenfd);
    return 0;
}
