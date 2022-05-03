#pragma once
#include "socket_optarg.h"
#include <netinet/in.h> //IPPROTO_IP
#include <sys/socket.h> //socket
enum BLOCK_STATUS
{
    NONBLOCK = 0,
    BLOCK = 1
};
class Socket_Base
{
protected:
    BLOCK_STATUS isblock; // 1为同步,0为异步
    int address_family;   //常用为AF_INET和AF_INET6，默认AF_INET
    int type;             //常用为SOCK_STREAM和SOCK_DGRAM，默认为SOCK_STREAM
    int protocol;         //一般设置为IPPROTO_IP(0)，让操作系统自己推演
    int socket_fd;        //打开的socketfd
    int rbuf, wbuf;

public:
    Socket_Base(Socket_optarg arg, BLOCK_STATUS __isblock = BLOCK, int __af = AF_INET, int __type = SOCK_STREAM, int __protocol = IPPROTO_IP);
    void Socket();
    int Getrbuf();
    int Getwbuf();
    void Setrbuf(int rbuf);
    void Setwbuf(int wbuf);
    void SetNonblock();
    int SetNonblocking(int fd);
    int Getsocketfd();
    ~Socket_Base();
};
