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
    BLOCK_STATUS isblock; // 1Ϊͬ��,0Ϊ�첽
    int address_family;   //����ΪAF_INET��AF_INET6��Ĭ��AF_INET
    int type;             //����ΪSOCK_STREAM��SOCK_DGRAM��Ĭ��ΪSOCK_STREAM
    int protocol;         //һ������ΪIPPROTO_IP(0)���ò���ϵͳ�Լ�����
    int socket_fd;        //�򿪵�socketfd
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
