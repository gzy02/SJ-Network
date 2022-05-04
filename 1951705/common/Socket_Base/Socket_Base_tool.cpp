#include "../../include/Socket_Base.h"
#include <cstdlib>      //exit
#include <iostream>     //cerr
#include <netinet/in.h> //IPPROTO_IP
#include <sys/fcntl.h>  //SetNonblock
#include <sys/socket.h> //socket

using namespace std;

//���ö�������
void Socket_Base::Setrbuf(int rbuf) // 0Ϊ�ɹ���-1Ϊʧ��
{
    socklen_t optlen = sizeof(socklen_t);
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &rbuf, optlen) == -1)
    {
        cerr << "Error: Setrbuf()" << endl;
        exit(EXIT_FAILURE);
    }
}
//����д������
void Socket_Base::Setwbuf(int wbuf) // 0Ϊ�ɹ���-1Ϊʧ��
{
    socklen_t optlen = sizeof(socklen_t);
    if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, &wbuf, optlen))
    {
        cerr << "Error: Setwbuf()" << endl;
        exit(EXIT_FAILURE);
    }
}
int Socket_Base::Getrbuf()
{
    socklen_t optlen = sizeof(socklen_t);
    int recvbuf;
    if (getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, (void *)&recvbuf, &optlen))
    {
        cerr << "Error: Getrbuf()" << endl;
        exit(EXIT_FAILURE);
    }
    return recvbuf;
}
int Socket_Base::Getwbuf()
{
    socklen_t optlen = sizeof(socklen_t);
    int sendbuf;
    if (getsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, (void *)&sendbuf, &optlen))
    {
        cerr << "Error: Getrbuf()" << endl;
        exit(EXIT_FAILURE);
    }
    return sendbuf;
}

int Socket_Base::SetNonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (-1 == flags)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Socket_Base::SetNonblock()
{
    if (isblock == BLOCK)
        if (SetNonblocking(socket_fd) == -1)
        {
            cerr << "Error: SetNonblock()" << endl;
            exit(EXIT_FAILURE);
        }
        else
            isblock = NONBLOCK;
}
