#pragma once
#include "Socket_Base.h"
#include "socket_optarg.h"
#include <netinet/in.h>
//用于Client端connect的socket的后续处理
class Socket_Connect : public Socket_Base
{
protected:
    sockaddr_in dest_addr;
    char serverIP[INET_ADDRSTRLEN];
    sockaddr_in my_addr;
    int sendbytes, readbytes;
    char *send_str;
    char *read_str;
    void Initread_str(int __readbytes);
    void Initsend_str(int __sendbytes);

public:
    Socket_Connect(Socket_optarg arg,
                   BLOCK_STATUS __isblock = BLOCK, int __af = AF_INET,
                   int __type = SOCK_STREAM, int __protocol = IPPROTO_IP);
    ~Socket_Connect();
    void Setsendbytes(int __sendbytes);
    void Setreadbytes(int __readbytes);
    void Setsend_str(const char *__send_str);
    const char *Getread_str();
    const char *GetserverIP();
    int GetserverPort();
    ssize_t Send(int __flags);
    ssize_t Write();
    ssize_t Send(const char *__buf, int __n, int __flags);
    ssize_t Write(const char *__buf, int __n);
    ssize_t Recv(int __flags);
    ssize_t Read();
};
